/**
 * @file error_codes.hpp
 * @brief error_code values the SDK client sets itself, without a bridge round trip.
 *
 * ServiceResponse.error_code is 0 on success. On failure it is a packed, always-positive
 * code: `error_code >> 16` is the package and `error_code & 0xFFFF` is the number inside it
 * (human form "<PACKAGE>-<number>", e.g. "SDK-0001", "BRIDGE-0003"). Package 7 is the robot's
 * bridge, which sets most codes; package 12 is this SDK.
 */

#ifndef __IGRIS_C_SDK_ERROR_CODES_HPP__
#define __IGRIS_C_SDK_ERROR_CODES_HPP__

#include <cstdint>

namespace igris_c_sdk::err {

inline constexpr std::int32_t kPackageSdk = 12;  // this SDK's package number

// No response arrived before the client timeout (SDK-0001).
inline constexpr std::int32_t kRequestTimeout = (kPackageSdk << 16) | 1;
// The request could not be published (SDK-0002).
inline constexpr std::int32_t kPublishFailed = (kPackageSdk << 16) | 2;

}  // namespace igris_c_sdk::err

#endif /* __IGRIS_C_SDK_ERROR_CODES_HPP__ */
