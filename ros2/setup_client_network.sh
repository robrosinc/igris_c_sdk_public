#!/bin/bash
# Host network tuning for receiving IGRIS-C camera image streams over ROS 2.
#
# Required on the CLIENT machine (the one running the subscriber), not on the
# robot. The robot only fragments; this host is the one that has to reassemble.
#
# Writes /etc/sysctl.d/99-igris-c-sdk-client.conf and applies it immediately.
# Re-runnable: the file is overwritten, never appended to, and no other sysctl
# file is edited. Uses sudo.
set -e

CONF="/etc/sysctl.d/99-igris-c-sdk-client.conf"

# 64MiB default / 128MiB max. A socket that does not set SO_RCVBUF gets
# net.core.rmem_default, and one that asks for more is capped at
# net.core.rmem_max, so both matter. Fast DDS leaves the size unset by
# default, which makes rmem_default the effective receive buffer.
RMEM_MAX=134217728
RMEM_DEFAULT=67108864

# The ROS 2 image lane sends ~64KiB UDP datagrams, which IP splits into ~40
# Ethernet frames each. The receiver holds the pieces in a kernel reassembly
# pool until the whole datagram is complete; one missing piece discards all of
# them. The stock 4MiB pool holds only ~190ms of a 21MB/s stream while
# incomplete datagrams squat in it for ipfrag_time seconds, so the pool hits
# its ceiling and evicts entries that were about to complete. Measured on a
# 21MB/s stream: IpReasmFails 213126 -> 620 per 20s after applying these.
IPFRAG_HIGH=268435456
IPFRAG_LOW=201326592
# 2s instead of the stock 30s: a video frame that old is useless anyway, and
# holding it only crowds out datagrams that can still complete.
IPFRAG_TIME=2

if [ ! -d /etc/sysctl.d ]; then
    echo "ERROR: /etc/sysctl.d not found; apply the values in $(basename "$0") by hand" >&2
    exit 1
fi

echo "Current values:"
sysctl net.core.rmem_max net.core.rmem_default \
       net.ipv4.ipfrag_high_thresh net.ipv4.ipfrag_low_thresh net.ipv4.ipfrag_time

echo
echo "Writing ${CONF}..."
sudo tee "${CONF}" >/dev/null <<EOF
# IGRIS-C SDK client: receiving camera image streams on the ROS 2 lane.
# Installed by igris_c_sdk_public/ros2/setup_client_network.sh -- re-run that
# script to refresh, or delete this file to revert to distribution defaults.

# Socket receive buffers (64MiB default, 128MiB max).
net.core.rmem_max = ${RMEM_MAX}
net.core.rmem_default = ${RMEM_DEFAULT}

# IP fragment reassembly pool. ~64KiB datagrams arrive as ~40 fragments each;
# the stock 4MiB pool thrashes at image-stream rates and drops datagrams that
# would otherwise have completed.
net.ipv4.ipfrag_high_thresh = ${IPFRAG_HIGH}
net.ipv4.ipfrag_low_thresh = ${IPFRAG_LOW}
net.ipv4.ipfrag_time = ${IPFRAG_TIME}
EOF
sudo chmod 644 "${CONF}"

sudo sysctl --system >/dev/null
echo
echo "Applied values:"
sysctl net.core.rmem_max net.core.rmem_default \
       net.ipv4.ipfrag_high_thresh net.ipv4.ipfrag_low_thresh net.ipv4.ipfrag_time
echo
echo "Done. Persists across reboots via ${CONF}."
echo "To verify while a stream runs:  nstat -az | grep IpReasm"
