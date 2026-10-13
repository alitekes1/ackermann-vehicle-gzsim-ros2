#include <chrono>
#include <memory>
#include <optional>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"

using namespace std::chrono_literals;

// Maps the left stick of a gamepad (/joy) to /cmd_vel.
// Publishes at a fixed rate and falls back to a zero command when /joy goes silent
// (controller unplugged, joy node crashed), so the vehicle never keeps its last command.
class JoystickController : public rclcpp::Node
{
public:
    JoystickController() : Node("joystick_controller_node")
    {
        timeout_ = std::chrono::duration<double>(declare_parameter("joy_timeout", 0.5));
        pub_ = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        sub_ = create_subscription<sensor_msgs::msg::Joy>(
            "joy", 10, [this](sensor_msgs::msg::Joy::ConstSharedPtr msg) {
                if (msg->axes.size() < 2) {
                    RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 5000,
                                         "Joy message has %zu axes, need at least 2", msg->axes.size());
                    return;
                }
                cmd_.linear.x = msg->axes[1];
                cmd_.angular.z = msg->axes[0];
                last_joy_ = now();
            });
        timer_ = create_wall_timer(50ms, [this]() {
            const bool stale = !last_joy_ || (now() - *last_joy_) > rclcpp::Duration(timeout_);
            pub_->publish(stale ? geometry_msgs::msg::Twist() : cmd_);
        });
    }

private:
    std::chrono::duration<double> timeout_;
    std::optional<rclcpp::Time> last_joy_;
    geometry_msgs::msg::Twist cmd_;
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JoystickController>());
    rclcpp::shutdown();
    return 0;
}
