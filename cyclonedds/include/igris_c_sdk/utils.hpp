/**
 * @file utils.hpp
 * @author type your name (type your email)
 * @brief
 * @version 0.1
 * @date 2026-05-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __UTILS_A8AF8E50_BE6D_4686_8ACA_53D85BF8C76F_H__
#define __UTILS_A8AF8E50_BE6D_4686_8ACA_53D85BF8C76F_H__

#include "igris_c_sdk/types.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace igris_c_sdk {

// CRC32 calculation for command validation
uint32_t crc32_core(const uint32_t *data, uint32_t len);

// Get current timestamp in microseconds
uint64_t get_timestamp_us();

// Linear interpolation for smooth transitions
float lerp(float start, float end, float t);

// Clamp value between min and max
float clamp(float value, float min, float max);

// Convert degrees to radians
float deg2rad(float deg);

// Convert radians to degrees
float rad2deg(float rad);

// ========== Helper Functions for Header ==========

// Fill header.stamp from the current system time. frame_id is left untouched.
// (The contract carries no seq; drop detection uses stamp monotonicity.)
void stamp_header(std_msgs::msg::dds_::Header_ &header);

// Explicit frame_id; stamp still comes from the current system time.
void stamp_header(std_msgs::msg::dds_::Header_ &header, const std::string &frame_id);

// Fully explicit: use when the sample carries its own acquisition time and stamping
// at publish time would discard it (per-sample sensor timestamps, replayed logs).
// stamp_ns is nanoseconds since the Unix epoch, matching the system clock the other
// overloads read, so the two remain comparable.
void stamp_header(std_msgs::msg::dds_::Header_ &header, const std::string &frame_id, uint64_t stamp_ns);

// ========== Helper Functions for MotorCmd ==========

// Create a MotorCmd with all parameters
igris_c_sdk::msg::dds_::MotorCmd_ create_motor_cmd(uint16_t motor_id, float q, float dq, float tau, float kp, float kd);

// Get motor state from LowState by motor index (MS mode)
const igris_c_sdk::msg::dds_::MotorState_ &get_motor_state(const igris_c_sdk::msg::dds_::LowState_ &state, uint16_t motor_id);

// Get joint state from LowState by joint index (PJS mode)
const igris_c_sdk::msg::dds_::JointState_ &get_joint_state(const igris_c_sdk::msg::dds_::LowState_ &state, uint16_t joint_id);

}  // namespace igris_c_sdk

#endif /* __UTILS_A8AF8E50_BE6D_4686_8ACA_53D85BF8C76F_H__ */
