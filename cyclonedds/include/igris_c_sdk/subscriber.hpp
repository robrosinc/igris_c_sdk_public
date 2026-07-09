/**
 * @file subscriber.hpp
 * @author type your name (type your email)
 * @brief
 * @version 0.1
 * @date 2026-05-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __SUBSCRIBER_E66D2DE9_3EBE_4198_9226_1A384923FE91_H__
#define __SUBSCRIBER_E66D2DE9_3EBE_4198_9226_1A384923FE91_H__

#include "igris_c_sdk/channel_factory.hpp"
#include "igris_c_sdk/qos.hpp"

#include <dds/dds.hpp>
#include <functional>
#include <memory>
#include <string>

namespace igris_c_sdk {

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

    Subscriber(const std::string &topic_name, const QosProfile &qos = {});
    Subscriber(ChannelFactory *channel_factory, const std::string &topic_name, const QosProfile &qos = {});
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
    std::string resolved_topic_;
    bool initialized_;
    ChannelFactory *channel_factory_;
    QosProfile qos_;

    std::shared_ptr<dds::sub::Subscriber> subscriber_;
    std::shared_ptr<dds::topic::Topic<MessageType>> topic_;
    std::shared_ptr<dds::sub::DataReader<MessageType>> reader_;
    std::shared_ptr<SubscriberListener<MessageType>> listener_;
};

}  // namespace igris_c_sdk

#endif /* __SUBSCRIBER_E66D2DE9_3EBE_4198_9226_1A384923FE91_H__ */
