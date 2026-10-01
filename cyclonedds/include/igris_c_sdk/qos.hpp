/**
 * @file qos.hpp
 * @author type your name (type your email)
 * @brief
 * @version 0.1
 * @date 2026-05-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __QOS_DA1F17E7_45AE_4E44_B2F9_8DCFF2A965A6_H__
#define __QOS_DA1F17E7_45AE_4E44_B2F9_8DCFF2A965A6_H__

#include <cstdint>

namespace igris_c_sdk {

struct QosProfile {
    enum class Reliability { Reliable, BestEffort };
    enum class Durability { Volatile, TransientLocal };
    enum class History { KeepLast, KeepAll };

    Reliability reliability = Reliability::Reliable;
    Durability durability   = Durability::Volatile;
    History history         = History::KeepLast;
    uint32_t depth          = 10;

    // ROS 2 rclcpp-aligned preset factories
    static QosProfile Default();     // ~ rclcpp::QoS(10)
    static QosProfile SensorData();  // ~ rclcpp::SensorDataQoS()
    static QosProfile Services();    // ~ rclcpp::ServicesQoS() / rmw_qos_profile_services_default
    static QosProfile Latched();     // ~ rclcpp::QoS(1).transient_local()
};

}  // namespace igris_c_sdk

#endif /* __QOS_DA1F17E7_45AE_4E44_B2F9_8DCFF2A965A6_H__ */
