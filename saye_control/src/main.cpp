#include "saye_control/joystick_controller_simulation.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Joystick_Controller>());
    rclcpp::shutdown();
    return 0;
}
