#pragma once

#include "igris_sdk/channel_factory.hpp"

#include <dds/dds.hpp>
#include <functional>
#include <memory>
#include <string>

namespace igris_sdk {

template <typename MessageType> class Subscriber;

template <typename MessageType> class SubscriberListener : public dds::sub::NoOpDataReaderListener<MessageType> {
  public:
    using CallbackType = std::function<void(const MessageType &)>;

    explicit SubscriberListener(CallbackType callback) : callback_(callback) {}

    void on_data_available(dds::sub::DataReader<MessageType> &reader) override {
        dds::sub::LoanedSamples<MessageType> samples = reader.take();
        for (const auto &sample : samples) {
            if (sample.info().valid() && callback_) {
                callback_(sample.data());
            }
        }
    }

  private:
    CallbackType callback_;
};

template <typename MessageType> class Subscriber {
  public:
    using CallbackType = std::function<void(const MessageType &)>;

    Subscriber(const std::string &topic_name);
    Subscriber(ChannelFactory *channel_factory, const std::string &topic_name);
    ~Subscriber();

    // Initialize DDS subscriber with callback (Cyclone DDS)
    // Note: ChannelFactory must be initialized before calling this
    bool init(CallbackType callback);

    // Stop subscriber and release DDS resources
    void stop();

    // Check if subscriber is initialized
    bool is_initialized() const { return initialized_; }

  private:
    std::string topic_name_;
    bool initialized_;
    ChannelFactory *channel_factory_;

    std::shared_ptr<dds::sub::Subscriber> subscriber_;
    std::shared_ptr<dds::topic::Topic<MessageType>> topic_;
    std::shared_ptr<dds::sub::DataReader<MessageType>> reader_;
    std::shared_ptr<SubscriberListener<MessageType>> listener_;
};

}  // namespace igris_sdk
