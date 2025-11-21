#pragma once

#include <dds/dds.hpp>
#include <memory>
#include <string>

namespace igris_sdk {

template <typename MessageType> class Publisher {
  public:
    Publisher(const std::string &topic_name);
    ~Publisher();

    // Initialize DDS publisher (Cyclone DDS)
    // Note: ChannelFactory must be initialized before calling this
    bool init();

    // Publish a message
    bool write(const MessageType &msg);

    // Check if publisher is initialized
    bool is_initialized() const { return initialized_; }

  private:
    std::string topic_name_;
    bool initialized_;

    std::shared_ptr<dds::pub::Publisher> publisher_;
    std::shared_ptr<dds::topic::Topic<MessageType>> topic_;
    std::shared_ptr<dds::pub::DataWriter<MessageType>> writer_;
};

}  // namespace igris_sdk
