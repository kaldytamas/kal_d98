#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

// A teknős pozíciójára iratkozik fel, és másodpercenként egyszer kiírja.
class PoseLoggerNode : public rclcpp::Node
{
public:
  PoseLoggerNode()
  : Node("pose_logger_node")
  {
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      "/turtle1/pose", 10,
      std::bind(&PoseLoggerNode::pose_callback, this, std::placeholders::_1));
    last_log_ = this->now();
    RCLCPP_INFO(this->get_logger(), "pose_logger_node elindult");
  }

private:
  void pose_callback(const turtlesim::msg::Pose & msg)
  {
    const auto now = this->now();
    // A teknős kb. 62 Hz-en küldi a pozíciót, ezért csak másodpercenként írunk ki.
    if ((now - last_log_).seconds() >= 1.0) {
      RCLCPP_INFO(
        this->get_logger(), "x=%.2f  y=%.2f  theta=%.2f",
        msg.x, msg.y, msg.theta);
      last_log_ = now;
    }
  }

  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
  rclcpp::Time last_log_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PoseLoggerNode>());
  rclcpp::shutdown();
  return 0;
}