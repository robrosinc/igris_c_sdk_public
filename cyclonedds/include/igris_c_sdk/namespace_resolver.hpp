/**
 * @file namespace_resolver.hpp
 * @author type your name (type your email)
 * @brief
 * @version 0.1
 * @date 2026-05-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __NAMESPACE_RESOLVER_AA01E0BB_DB23_429A_A36E_D24719A2EC51_H__
#define __NAMESPACE_RESOLVER_AA01E0BB_DB23_429A_A36E_D24719A2EC51_H__

#include <string>

namespace igris_c_sdk {

/**
 * @brief Sanitizes a namespace token by replacing invalid characters with underscores. Valid characters are alphanumeric and underscore.
 *
 * @param raw
 * @return std::string
 */
std::string sanitize_namespace_token(const std::string &raw);

/**
 * @brief Reads the machine ID prefix from /etc/machine-id. The machine ID is typically a 32-character hexadecimal string, and this function
 * returns the first 8 characters to be used as a namespace suffix. If the file cannot be read or is invalid, it returns an empty string.
 *
 * @return std::string
 */
std::string read_machine_id_prefix();

/**
 * @brief Resolves the robot namespace used as a DDS topic prefix based on the given suffix policy and user suffix.
 *
 * @param suffix_policy The policy for determining the namespace suffix. Options are "ap_suffix", "serial_suffix", "custom", "auto", and
 * "none".
 * @param user_suffix The user-defined suffix to use if the suffix policy is "custom".
 * @return std::string The resolved robot namespace.
 */
std::string resolve_robot_namespace(const std::string &suffix_policy = "ap_suffix", const std::string &user_suffix = "");

/**
 * @brief Layout of a topic name for one transport lane.
 *
 * The rt/ segment exists only because ROS 2 puts it there: rmw mangles a node's topic /<ns>/<topic> into the DDS name rt/<ns>/<topic>.
 * Native CycloneDDS has no use for it, so Native carries none. RosCompatible reproduces the rmw result exactly and is therefore the layout
 * a rclcpp subscriber can reach - verified by measurement: a raw CycloneDDS writer using it is received by a rclcpp node on the DEFAULT
 * (FastDDS) rmw, with no rmw switch required.
 *
 * Native and RosCompatible are not interchangeable at runtime: a robot and a client that disagree never discover each other (silent, not an
 * error), so both sides must change together.
 */
enum class TopicNaming {
    Ros,           ///< /<ns>/<topic>   - ROS node-side name; rmw prepends rt/ itself
    Native,        ///< <ns>/<topic>    - native CycloneDDS; no rt/, which is a ROS artifact
    RosCompatible  ///< rt/<ns>/<topic> - native layout matching ROS 2 rmw mangling
};

/**
 * @brief Lays out a topic name for the given namespace and transport lane. This is the single implementation every repository must use;
 * callers pass the BARE topic ("lowstate", "service/torque/request") because the rt/ segment belongs to the layout, not to the caller.
 *
 * A leading '/' on @p topic is stripped rather than honoured: under the 1-robot / 1-namespace policy a caller must not be able to escape
 * the namespace by writing an absolute-looking name.
 *
 * @param ns Robot namespace, typically from resolve_robot_namespace(). Empty disables the namespace segment.
 * @param topic Bare topic name.
 * @param naming Lane layout; see TopicNaming.
 * @return std::string The resolved topic name.
 */
std::string resolve_topic_name(const std::string &ns, const std::string &topic, TopicNaming naming);

/**
 * @brief Parses a native-lane wire layout ("native" / "ros_compatible") into a TopicNaming.
 *
 * This is the layout half of the robot's igris_c.network.transport, which split_transport() hands over: cyclonedds_native yields "native",
 * cyclonedds_ros_compatible yields "ros_compatible". Every native-lane process must agree with the robot on it - they discover each other
 * only if they match, and a mismatch is silent rather than an error. Parsing lives here, once, so a consumer cannot drift from the bridge
 * by reimplementing it.
 *
 * An unrecognised value falls back to Native and reports it through @p warning rather than throwing: a typo in robot.yaml must not leave a
 * process on an unpredictable wire layout, and it must not stop the robot from starting either.
 *
 * @param value Config string; "native" or "ros_compatible". Empty means "not configured" and defaults quietly.
 * @param warning If non-null, receives a human-readable message when @p value was not recognised.
 * @return TopicNaming The parsed layout, or TopicNaming::Native.
 */
TopicNaming parse_topic_naming(const std::string &value, std::string *warning = nullptr);

/**
 * @brief Splits the igris_c.network.transport config value into the lane a process serves and the
 * native lane's wire layout.
 *
 * One key carries both because the wire layout exists only on the cyclonedds lane: under "ros" the
 * rmw forces the rt/ prefix, so there is nothing to choose. The three values are therefore the
 * three that exist: "cyclonedds_native", "cyclonedds_ros_compatible", "ros".
 *
 * Older values ("cyclonedds", "both") are mapped rather than rejected: a config that still carries
 * one must not leave the robot with no external transport at all. They resolve to the shipping
 * default and say so through @p warning, distinguished from a typo so an operator is not sent
 * hunting for a misspelling.
 *
 * @param value Config string. Empty means "not configured" and defaults quietly.
 * @param transport Receives "cyclonedds" or "ros". May be null.
 * @param topic_naming Receives "native" or "ros_compatible"; meaningless when the lane is "ros". May be null.
 * @param warning If non-null, receives a message when @p value was legacy or unrecognised.
 */
void split_transport(const std::string &value, std::string *transport, std::string *topic_naming, std::string *warning = nullptr);

/**
 * @brief Extracts the "--dds-topic-naming <value>" flag from a command line.
 *
 * The flag spelling is part of the layout contract, not of any one program: the camera node, the SDK examples and anything else launched by
 * hand all have to accept the same one, or an operator has to remember which binary spells it which way. It lives here for the same reason
 * parse_topic_naming() does.
 *
 * @param argc,argv Command line as received by main().
 * @return std::string The flag's value, or "" when absent - which parse_topic_naming() reads as "not configured".
 */
std::string topic_naming_arg(int argc, char **argv);

/**
 * @brief True if argv[index] is the "--dds-topic-naming" flag or the value directly after it.
 *
 * For callers that treat trailing positionals as data (topic names, file paths): without this the flag and its value are consumed as two
 * bogus entries.
 */
bool is_topic_naming_arg(int argc, char **argv, int index);

/**
 * @brief Convenience: topic_naming_arg() followed by parse_topic_naming().
 *
 * @param argc,argv Command line as received by main().
 * @param warning If non-null, receives a human-readable message when the flag value was not recognised.
 * @return TopicNaming The parsed layout, or TopicNaming::Native when the flag is absent or unrecognised.
 */
TopicNaming parse_topic_naming_args(int argc, char **argv, std::string *warning = nullptr);

/**
 * @brief The config string for a layout, for logs.
 *
 * A layout mismatch produces no error anywhere, so the only thing an operator can compare against the robot's config is what each side
 * prints. Naming that string once keeps the two comparable.
 *
 * Only "native" and "ros_compatible" round-trip through parse_topic_naming(). TopicNaming::Ros returns "ros", which is not a
 * dds_topic_naming value at all - that lane is a ROS node's own topic name, never a config choice - so parsing it back yields Native and
 * warns. Use this for logging, not as a serialisation format.
 */
const char *to_config_string(TopicNaming naming);

/**
 * @brief Usage text for the --dds-topic-naming flag, for a program's own usage block.
 */
const char *topic_naming_usage();

}  // namespace igris_c_sdk

#endif /* __NAMESPACE_RESOLVER_AA01E0BB_DB23_429A_A36E_D24719A2EC51_H__ */
