# IGRIS-C SDK — Python

Python bindings for the IGRIS-C robot. The wheel talks DDS through the same
bundled Cyclone DDS runtime as `cyclonedds/`, so no ROS 2 install is needed.

## Layout

```text
python/
├── pixi.toml, pixi.lock      Reproducible env (Python 3.12 + numpy/opencv/dearpygui)
├── cyclonedds_spdp.xml       DDS config, multicast discovery (start here)
├── cyclonedds_peers.xml      DDS config, unicast — when multicast is blocked
├── common.py                 Shared helpers used by the examples
├── client.ipynb              Notebook walkthrough
└── *.py                      Example clients
```

The two `cyclonedds_*.xml` files are the same ones in `cyclonedds/`, copied here
on purpose: this subtree stands on its own, so nothing points you at `../cyclonedds/`.

## Install

The wheel is in `../dist/`. Either use the bundled pixi env:

```bash
# Install pixi once: https://pixi.sh/latest/
cd python
pixi install
pixi run install-wheel        # pip install ../dist/*.whl
pixi shell                    # activate
```

or a plain virtualenv:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install ../dist/igris_c_sdk-*.whl
pip install numpy opencv-python dearpygui   # only for the viewers / GUI examples
```

## Run

Export `CYCLONEDDS_URI` first — discovery finds nothing without it. Both sample
configs live in this directory, so run this from `python/`:

```bash
export CYCLONEDDS_URI="file://$PWD/cyclonedds_spdp.xml"
```

Edit `<NetworkInterface name="...">` in that file to the interface on THIS machine
that reaches the robot (`ip link` lists them). Use `cyclonedds_peers.xml` instead
when multicast is blocked; it additionally needs the robot's address in `<Peer address="...">`.

Every example takes `--domain-id` and `--namespace`, plus an optional
`--dds-topic-naming native|ros_compatible` that must match the robot's
`igris_c.network.transport` (`native` pairs with `cyclonedds_native`,
`ros_compatible` with `cyclonedds_ros_compatible`). A mismatch is silent: the two
sides never discover each other. See the root README's
[Topic layout](../README.md#topic-layout).

| Script | Purpose | pixi task |
| --- | --- | --- |
| `client.py` | Service API menu + low-level control | `pixi run client` |
| `hand_example.py` | Headless end-effector (hand) control | — |
| `twist_publisher.py` | Walk-velocity (`Twist`) publisher | — |
| `sensor_viewer.py` | Sensor visualization (needs numpy + opencv) | `pixi run sensor-viewer` |
| `sdk_gui_client.py` | Full-featured GUI client (Dear PyGui) | `pixi run gui` |
| `sdk_hand_gui_client.py` | Hand control GUI client | `pixi run hand-gui` |
| `client.ipynb` | Notebook walkthrough | — |

```bash
python3 client.py --domain-id 0 --namespace igris_c_<your_robot_id>
```

`client.py` prints a numbered service menu on every iteration. The C++
(`../cyclonedds/cyclonedds_service`) and ROS 2 (`../ros2` → `ros_service`) examples
use the SAME numbering, so a sequence written against one applies to all three.

> `client.ipynb` does not take `--dds-topic-naming`; it is pinned to the default
> `native` layout. Use it only against a robot on `cyclonedds_native`.
