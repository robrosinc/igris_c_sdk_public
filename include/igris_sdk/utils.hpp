#pragma once

#include "igris_sdk/types.hpp"

#include <cstdint>
#include <string>

namespace igris_sdk {

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

// ========== Helper Functions for MotorCmd ==========

// Create a MotorCmd with all parameters
MotorCmd create_motor_cmd(uint16_t motor_id, float q, float dq, float tau, float kp, float kd);

// Get motor state from LowState by motor index (MS mode)
const MotorState &get_motor_state(const LowState &state, uint16_t motor_id);

// Get joint state from LowState by joint index (PJS mode)
const JointState &get_joint_state(const LowState &state, uint16_t joint_id);

}  // namespace igris_sdk
