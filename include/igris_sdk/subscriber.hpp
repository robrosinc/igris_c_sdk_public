#pragma once

#include <atomic>
#include <dds/dds.hpp>
#include <functional>
#include <memory>
#include <string>
#include <thread>

namespace igris_sdk {

template <typename MessageType> class Subscriber {
  public:
    using CallbackType = std::function<void(const MessageType &)>;

    Subscriber(const std::string &topic_name);
    ~Subscriber();

    // Initialize DDS subscriber with callback (Cyclone DDS)
    // Note: ChannelFactory must be initialized before calling this
    // Automatically starts listening after initialization
    bool init(CallbackType callback);

    // Stop listening (can be restarted with start())
    void stop();

    // Start listening (called automatically by init())
    bool start();

    // Check if subscriber is initialized
    bool is_initialized() const { return initialized_; }

    // Check if subscriber is running
    bool is_running() const { return running_; }

  private:
    void listenerThread();

    std::string topic_name_;
    bool initialized_;
    CallbackType callback_;

    std::shared_ptr<dds::sub::Subscriber> subscriber_;
    std::shared_ptr<dds::topic::Topic<MessageType>> topic_;
    std::shared_ptr<dds::sub::DataReader<MessageType>> reader_;

    std::thread listener_thread_;
    std::atomic<bool> running_;
};

}  // namespace igris_sdk
