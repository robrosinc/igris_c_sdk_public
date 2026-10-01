/**
 * @file channel_factory.hpp
 * @author type your name (type your email)
 * @brief
 * @version 0.1
 * @date 2026-05-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __CHANNEL_FACTORY_DA53A731_1BD4_407C_AD28_D845ED2C029F_H__
#define __CHANNEL_FACTORY_DA53A731_1BD4_407C_AD28_D845ED2C029F_H__

#include "igris_c_sdk/namespace_resolver.hpp"

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
    // ns: prefix applied to every topic (e.g. "robot1" turns "lowcmd" into
    //     "robot1/lowcmd" under Native, "rt/robot1/lowcmd" under
    //     RosCompatible). Empty string disables the namespace segment.
    // config: optional Cyclone DDS XML config string.
    // naming: wire layout for topic names; see TopicNaming.
    void Init(int32_t domain_id = 0, const std::string &ns = "", const std::string &config = "", TopicNaming naming = TopicNaming::Native);

    bool IsInitialized() const { return initialized_; }

    // Returns nullptr if not initialized.
    std::shared_ptr<dds::domain::DomainParticipant> GetParticipant();

    int32_t GetDomainId() const { return domain_id_; }
    const std::string &GetNamespace() const { return namespace_; }
    TopicNaming GetTopicNaming() const { return naming_; }

    // Lay out a topic name on the wire with the configured namespace and
    // TopicNaming. Thin wrapper over resolve_topic_name(); see there for the
    // rules callers must follow.
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
    TopicNaming naming_ = TopicNaming::Native;
    std::shared_ptr<dds::domain::DomainParticipant> participant_;
    std::mutex participant_mutex_;
};

}  // namespace igris_c_sdk

#endif /* __CHANNEL_FACTORY_DA53A731_1BD4_407C_AD28_D845ED2C029F_H__ */
