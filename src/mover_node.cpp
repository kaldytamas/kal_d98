#include <chrono>
#include <cmath>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

// Négyzet alakban mozgatja a teknőst: egyenes szakasz, majd 90 fokos fordulás.
class MoverNode : public rclcpp::Node
{
public:
  MoverNode()
  : Node("mover_node")
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    timer_ = this->create_wall_timer(
      100ms, std::bind(&MoverNode::timer_callback, this));

    constexpr double kPi = 3.14159265358979323846;
    forward_ticks_ = static_cast<int>(std::round(2.0 / dt_));
    turn_ticks_ = static_cast<int>(std::round((kPi / 2.0) / angular_speed_ / dt_));

    RCLCPP_INFO(this->get_logger(), "mover_node elindult");
  }

private:
  void timer_callback()
  {
    auto msg = geometry_msgs::msg::Twist();
    int limit = 0;
    if (turning_) {
      msg.angular.z = angular_speed_;
      limit = turn_ticks_;
    } else {
      msg.linear.x = linear_speed_;
      limit = forward_ticks_;
    }
    publisher_->publish(msg);

    tick_++;
    if (tick_ >= limit) {
      tick_ = 0;
      turning_ = !turning_;
    }
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  double linear_speed_ = 1.0;   // egyenes szakasz sebessége
  double angular_speed_ = 1.0;  // fordulási sebesség (rad/s)
  double dt_ = 0.1;             // timer periódusa (s)
  int forward_ticks_ = 0;
  int turn_ticks_ = 0;
  int tick_ = 0;
  bool turning_ = false;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MoverNode>());
  rclcpp::shutdown();
  return 0;
}