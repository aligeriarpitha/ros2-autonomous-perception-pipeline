#ifndef ROS2_AUTONOMOUS_PERCEPTION_PIPELINE__OBSTACLE_FILTER_NODE_HPP_
#define ROS2_AUTONOMOUS_PERCEPTION_PIPELINE__OBSTACLE_FILTER_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

class ObstacleFilterNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit ObstacleFilterNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

  using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

  CallbackReturn on_configure(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_activate(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_shutdown(const rclcpp_lifecycle::State & state) override;

private:
  void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp_lifecycle::LifecyclePublisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;

  double min_range_{0.2};
  double max_range_{10.0};
};

#endif  // ROS2_AUTONOMOUS_PERCEPTION_PIPELINE__OBSTACLE_FILTER_NODE_HPP_
