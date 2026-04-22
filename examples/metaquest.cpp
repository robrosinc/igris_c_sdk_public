#include "igris_sdk/igris_c_msgs.hpp"
#include "igris_sdk/types.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <dds/dds.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace {

using QuestMessage = igris_sdk::QuestMessage;

constexpr int kDefaultDomainId      = 0;
constexpr const char *kDefaultTopic = "igris_c/metaquest";

std::atomic<bool> g_stop{false};

void handle_signal(int) { g_stop.store(true); }

template <size_t N> std::string format_array(const std::array<float, N> &values, int precision = 3) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << "[";
    for (size_t i = 0; i < N; ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << values[i];
    }
    oss << "]";
    return oss.str();
}

std::string format_bool(bool value) { return value ? "Y" : "N"; }

template <size_t N> std::string format_bool_array(const std::array<bool, N> &values) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < N; ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << format_bool(values[i]);
    }
    oss << "]";
    return oss.str();
}

bool uses_controller_tracking(int tracking_mode) { return tracking_mode == 0; }

bool uses_hand_tracking(int tracking_mode) { return tracking_mode == 1; }

void print_rule(size_t width);
void print_kv(const std::string &key, const std::string &value);
void print_usage(const char *program);

std::string format_buttons(const igris_sdk::QuestController &controller) {
    std::ostringstream oss;
    oss << "1:" << format_bool(controller.button_one()) << " " << "2:" << format_bool(controller.button_two()) << " "
        << "3:" << format_bool(controller.button_three()) << " " << "4:" << format_bool(controller.button_four()) << " "
        << "JC:" << format_bool(controller.joystick_click()) << " " << "IT:" << format_bool(controller.index_trigger_touch()) << " "
        << "HT:" << format_bool(controller.hand_trigger_touch());
    return oss.str();
}

std::string format_finger_names() { return "[thumb, index, middle, ring, pinky]"; }

void print_controller_section(size_t index, const igris_sdk::QuestController &controller) {
    const std::string prefix = "Ctrl[" + std::to_string(index) + "]";
    print_kv(prefix + " Pos", format_array(controller.position().data()));
    print_kv(prefix + " Euler", format_array(controller.euler().data()));
    print_kv(prefix + " Quat", format_array(controller.quaternion().data()));
    print_kv(prefix + " Lin Vel", format_array(controller.linear_velocity()));
    print_kv(prefix + " Ang Vel", format_array(controller.angular_velocity()));
    print_kv(prefix + " Joystick", format_array(controller.joystick()));
    print_kv(prefix + " Touchpad", format_array(controller.touchpad()));
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << controller.index_trigger();
        print_kv(prefix + " Index Trg", oss.str());
    }
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << controller.hand_trigger();
        print_kv(prefix + " Hand Trg", oss.str());
    }
    print_kv(prefix + " Buttons", format_buttons(controller));
}

void print_hand_section(size_t index, const igris_sdk::QuestHand &hand) {
    const std::string prefix = "Hand[" + std::to_string(index) + "]";
    print_kv(prefix + " Pos", format_array(hand.position().data()));
    print_kv(prefix + " Euler", format_array(hand.euler().data()));
    print_kv(prefix + " Quat", format_array(hand.quaternion().data()));
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << hand.confidence();
        print_kv(prefix + " Conf", oss.str());
    }
    print_kv(prefix + " Pinch", format_bool_array(hand.pinch()));
    print_kv(prefix + " Tip Valid", format_bool_array(hand.tip_valid()));
    print_kv(prefix + " Fingers", format_finger_names());
    print_rule(108);
    std::cout << prefix << " Finger Table\n";
    print_rule(108);
    std::cout << std::left << std::setw(8) << "Finger" << " | " << std::setw(5) << "Pinch" << " | " << std::setw(8) << "Strength" << " | "
              << std::setw(8) << "Conf" << " | " << std::setw(8) << "Valid" << " | " << std::setw(10) << "Dist" << " | " << std::setw(22)
              << "Tip Pos" << " | " << "Tip Quat" << "\n";
    print_rule(108);

    static constexpr const char *finger_names[5] = {"thumb", "index", "middle", "ring", "pinky"};
    for (size_t i = 0; i < 5; ++i) {
        std::ostringstream strength;
        std::ostringstream confidence;
        std::ostringstream distance;
        strength << std::fixed << std::setprecision(3) << hand.pinch_strength()[i];
        confidence << std::fixed << std::setprecision(3) << hand.finger_confidence()[i];
        distance << std::fixed << std::setprecision(3) << hand.tip_to_thumb_distance()[i];

        std::cout << std::left << std::setw(8) << finger_names[i] << " | " << std::setw(5) << format_bool(hand.pinch()[i]) << " | "
                  << std::setw(8) << strength.str() << " | " << std::setw(8) << confidence.str() << " | " << std::setw(8)
                  << format_bool(hand.tip_valid()[i]) << " | " << std::setw(10) << distance.str() << " | " << std::setw(22)
                  << format_array(hand.tip_position()[i].data()) << " | " << format_array(hand.tip_quaternion()[i].data()) << "\n";
    }
}

void print_controller_payload(const igris_sdk::QuestController &controller) { print_controller_section(0, controller); }

template <size_t N> void print_controller_payload(const std::array<igris_sdk::QuestController, N> &controllers) {
    for (size_t i = 0; i < controllers.size(); ++i) {
        print_controller_section(i, controllers[i]);
        if (i + 1 < controllers.size()) {
            print_rule(108);
        }
    }
}

void print_hand_payload(const igris_sdk::QuestHand &hand) {
    print_hand_section(0, hand);
    print_rule(108);
}

template <size_t N> void print_hand_payload(const std::array<igris_sdk::QuestHand, N> &hands) {
    for (size_t i = 0; i < hands.size(); ++i) {
        print_hand_section(i, hands[i]);
        if (i + 1 < hands.size()) {
            print_rule(108);
        }
    }
    print_rule(108);
}

struct Snapshot {
    bool has_message = false;
    QuestMessage message;
    uint64_t message_count                             = 0;
    std::chrono::steady_clock::time_point last_receive = std::chrono::steady_clock::now();
};

void print_rule(size_t width) { std::cout << std::string(width, '=') << "\n"; }

void print_kv(const std::string &key, const std::string &value) {
    std::cout << std::left << std::setw(20) << key << " | " << value << "\n";
}

void print_snapshot(int domain_id, const std::string &topic, const Snapshot &snapshot) {
    std::cout << "\x1B[2J\x1B[H";
    print_rule(108);
    std::cout << "MetaQuest Raw Viewer\n";
    print_rule(108);
    print_kv("Domain", std::to_string(domain_id));
    print_kv("Topic", topic);

    if (!snapshot.has_message) {
        print_kv("Status", "Waiting for QuestMessage samples...");
        print_kv("Quit", "Ctrl+C");
        print_rule(108);
        std::cout << std::flush;
        return;
    }

    const auto age_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - snapshot.last_receive).count();
    const auto &message = snapshot.message;
    const auto &header  = message.header();

    {
        std::ostringstream oss;
        oss << snapshot.message_count;
        print_kv("Message Count", oss.str());
    }
    print_kv("Age", std::to_string(age_ms) + " ms");
    print_kv("Seq", std::to_string(header.seq()));
    {
        std::ostringstream oss;
        oss << header.sec() << "." << std::setw(9) << std::setfill('0') << header.nanosec() << std::setfill(' ');
        print_kv("Stamp", oss.str());
    }
    print_kv("Frame ID", header.frame_id());
    print_kv("IP", message.ip());
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << message.battery();
        print_kv("Battery", oss.str());
    }
    print_kv("Tracking Mode", std::to_string(message.tracking_mode()));
    if (uses_controller_tracking(message.tracking_mode())) {
        print_kv("Active Input", "controller");
    } else if (uses_hand_tracking(message.tracking_mode())) {
        print_kv("Active Input", "hand");
    } else {
        print_kv("Active Input", "unknown");
    }
    print_rule(108);
    std::cout << "Pose Summary\n";
    print_rule(108);
    print_kv("Head Pos", format_array(message.head_position().data()));
    print_kv("Head Euler", format_array(message.head_euler().data()));
    print_kv("Head Quat", format_array(message.head_quaternion().data()));
    if (uses_controller_tracking(message.tracking_mode())) {
        print_controller_payload(message.controller());
    } else if (uses_hand_tracking(message.tracking_mode())) {
        print_hand_payload(message.hand());
    } else {
        print_kv("Tracking Data", "Unsupported tracking_mode");
    }
    print_rule(108);
    print_kv("Quit", "Ctrl+C");
    print_rule(108);
    std::cout << std::flush;
}

bool parse_domain_id(const char *value, int &domain_id) {
    char *end   = nullptr;
    long parsed = std::strtol(value, &end, 10);
    if (end == value || *end != '\0') {
        return false;
    }
    domain_id = static_cast<int>(parsed);
    return true;
}

bool parse_arguments(int argc, char **argv, int &domain_id, std::string &topic) {
    bool domain_id_set = false;
    bool topic_set     = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            std::exit(0);
        }

        if (arg == "--domain-id") {
            if (i + 1 >= argc || !parse_domain_id(argv[++i], domain_id)) {
                std::cerr << "Invalid --domain-id value\n";
                return false;
            }
            domain_id_set = true;
            continue;
        }

        if (arg == "--topic") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --topic\n";
                return false;
            }
            topic     = argv[++i];
            topic_set = true;
            continue;
        }

        if (!domain_id_set && parse_domain_id(arg.c_str(), domain_id)) {
            domain_id_set = true;
            continue;
        }

        if (!topic_set) {
            topic     = arg;
            topic_set = true;
            continue;
        }

        std::cerr << "Unexpected argument: " << arg << "\n";
        return false;
    }

    return true;
}

void print_usage(const char *program) {
    std::cout << "Usage: " << program << " [--domain-id N] [--topic TOPIC]\n"
              << "       " << program << " [domain_id] [topic]\n"
              << "Defaults: domain_id=" << kDefaultDomainId << ", topic=" << kDefaultTopic << "\n";
}

}  // namespace

int main(int argc, char **argv) {
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    int domain_id     = kDefaultDomainId;
    std::string topic = kDefaultTopic;

    if (!parse_arguments(argc, argv, domain_id, topic)) {
        print_usage(argv[0]);
        return 1;
    }

    try {
        dds::domain::DomainParticipant participant(domain_id);
        dds::topic::Topic<QuestMessage> dds_topic(participant, topic);
        dds::sub::Subscriber subscriber(participant);

        dds::sub::qos::DataReaderQos reader_qos = subscriber.default_datareader_qos();
        reader_qos << dds::core::policy::Reliability::Reliable() << dds::core::policy::History::KeepLast(10);
        dds::sub::DataReader<QuestMessage> reader(subscriber, dds_topic, reader_qos);

        Snapshot snapshot;
        while (!g_stop.load()) {
            auto samples = reader.take();
            for (const auto &sample : samples) {
                if (!sample.info().valid()) {
                    continue;
                }

                snapshot.message = sample.data();
                snapshot.message_count++;
                snapshot.last_receive = std::chrono::steady_clock::now();
                snapshot.has_message  = true;
            }

            print_snapshot(domain_id, topic, snapshot);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const dds::core::Exception &e) {
        std::cerr << "Failed to initialize DDS subscriber for topic " << topic << ": " << e.what() << "\n";
        return 1;
    }
    return 0;
}
