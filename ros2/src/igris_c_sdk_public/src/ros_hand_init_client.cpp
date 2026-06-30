#include "common.hpp"

#include <igris_c_sdk/srv/hand_init_request.hpp>
#include <rclcpp/rclcpp.hpp>

using igris_c_sdk_examples::g_running;
using igris_c_sdk_examples::signalHandler;

int main(int argc, char **argv) {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    igris_c_sdk_examples::initRclcpp(argc, argv);
    auto node                         = rclcpp::Node::make_shared("igris_c_ros_hand_init_client");
    const std::string robot_namespace = igris_c_sdk_examples::resolveRobotNamespace(node);
    auto client                       = node->create_client<igris_c_sdk::srv::HandInitRequest>(
        igris_c_sdk_examples::resolveRtName(robot_namespace, "rt/service/hand_init"));

    if (!client->wait_for_service(std::chrono::seconds(3))) {
        std::cerr << "Hand init service not available" << std::endl;
        rclcpp::shutdown();
        return 1;
    }

    auto req        = std::make_shared<igris_c_sdk::srv::HandInitRequest::Request>();
    req->request_id = igris_c_sdk_examples::makeRequestId("hand_init");

    auto future = client->async_send_request(req);
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(node);
    if (exec.spin_until_future_complete(future, std::chrono::seconds(10)) != rclcpp::FutureReturnCode::SUCCESS) {
        std::cerr << "Hand init request timeout" << std::endl;
        rclcpp::shutdown();
        return 1;
    }

    auto res = future.get();
    std::cout << "Hand init result: " << (res->success ? "OK" : "FAIL") << " msg=" << res->message << std::endl;
    rclcpp::shutdown();
    return res->success ? 0 : 1;
}
