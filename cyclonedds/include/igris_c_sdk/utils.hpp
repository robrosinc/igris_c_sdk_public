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

// Increment header.seq by 1 and fill header.sec/nanosec from current system time.
// frame_id is left untouched (mirrors shm Header::update() convention).
void stamp_header(Header &header);

// ========== Helper Functions for MotorCmd ==========

// Create a MotorCmd with all parameters
MotorCmd create_motor_cmd(uint16_t motor_id, float q, float dq, float tau, float kp, float kd);

// Get motor state from LowState by motor index (MS mode)
const MotorState &get_motor_state(const LowState &state, uint16_t motor_id);

// Get joint state from LowState by joint index (PJS mode)
const JointState &get_joint_state(const LowState &state, uint16_t joint_id);

}  // namespace igris_c_sdk

#endif /* __UTILS_A8AF8E50_BE6D_4686_8ACA_53D85BF8C76F_H__ */
