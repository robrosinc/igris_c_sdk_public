#pragma once

#include <dds/dds.hpp>
#include <memory>
#include <mutex>
#include <string>

namespace igris_sdk {

/**
 * @brief ChannelFactory singleton for managing shared DDS resources
 *
 * Inspired by Unitree's ChannelFactory pattern, this class provides:
 * - Single DomainParticipant shared across all publishers/subscribers
 * - Centralized DDS configuration (domain ID)
 * - Resource efficiency (avoid creating multiple DomainParticipants)
 */
class ChannelFactory {
  public:
    /**
     * @brief Get the singleton instance
     */
    static ChannelFactory *Instance();

    /**
     * @brief Initialize the factory with basic parameters
     * @param domain_id DDS domain ID (default: 0)
     */
    void Init(int32_t domain_id = 0);

    /**
     * @brief Check if factory is initialized
     */
    bool IsInitialized() const { return initialized_; }

    /**
     * @brief Get the shared DomainParticipant
     * @return Shared pointer to DomainParticipant (nullptr if not initialized)
     */
    std::shared_ptr<dds::domain::DomainParticipant> GetParticipant();

    /**
     * @brief Get domain ID
     */
    int32_t GetDomainId() const { return domain_id_; }

    /**
     * @brief Release all resources
     */
    void Release();

    // Delete copy/move constructors
    ChannelFactory(const ChannelFactory &)            = delete;
    ChannelFactory &operator=(const ChannelFactory &) = delete;
    ChannelFactory(ChannelFactory &&)                 = delete;
    ChannelFactory &operator=(ChannelFactory &&)      = delete;

    ChannelFactory();
    ~ChannelFactory();

  private:
    static ChannelFactory *instance_;
    static std::mutex instance_mutex_;

    bool initialized_;
    int32_t domain_id_;

    std::shared_ptr<dds::domain::DomainParticipant> participant_;
    std::mutex participant_mutex_;
};

}  // namespace igris_sdk
