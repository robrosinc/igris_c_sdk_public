#pragma once

#include <dds/dds.hpp>
#include <memory>
#include <mutex>
#include <string>

namespace igris_c_sdk {

// Singleton that owns the process-wide DomainParticipant and topic namespace.
// Init() must be called once before any Publisher/Subscriber is constructed.
class ChannelFactory {
  public:
    static ChannelFactory *Instance();

    // Initialize the shared DomainParticipant.
    // ns: prefix applied to every topic (e.g. "robot1" turns "rt/lowcmd" into
    //     "robot1/rt/lowcmd"). Empty string disables prefixing.
    // config: optional Cyclone DDS XML config string.
    void Init(int32_t domain_id = 0, const std::string &ns = "", const std::string &config = "");

    bool IsInitialized() const { return initialized_; }

    // Returns nullptr if not initialized.
    std::shared_ptr<dds::domain::DomainParticipant> GetParticipant();

    int32_t GetDomainId() const { return domain_id_; }
    const std::string &GetNamespace() const { return namespace_; }

    // Apply the configured namespace to a topic name. 1-robot/1-namespace
    // policy: a leading '/' is stripped and ns is still prepended so that a
    // stray "/rt/lowcmd" cannot bypass isolation.
    std::string resolve(const std::string &topic) const;

    // Release all DDS resources.
    void Release();

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
    std::string namespace_;
    std::shared_ptr<dds::domain::DomainParticipant> participant_;
    std::mutex participant_mutex_;
};

}  // namespace igris_c_sdk
