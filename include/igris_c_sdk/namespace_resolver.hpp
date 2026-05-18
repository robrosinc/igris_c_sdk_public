#pragma once

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

}  // namespace igris_c_sdk
