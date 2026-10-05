#ifndef ROS2_AUTONOMOUS_PERCEPTION_PIPELINE__OBSTACLE_FILTER_NODE_HPP_
#define ROS2_AUTONOMOUS_PERCEPTION_PIPELINE__OBSTACLE_FILTER_NODE_HPP_

#include <cstddef>
#include <memory>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

namespace perception
{

class ObstacleFilterNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit ObstacleFilterNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(
    const rclcpp_lifecycle::State & previous_state) override;
  
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(
    const rclcpp_lifecycle::State & previous_state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & previous_state) override;

private:
  void scanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp_lifecycle::LifecyclePublisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
  rclcpp::CallbackGroup::SharedPtr cb_group_;
  double distance_threshold_;
};

}  // namespace perception

#endif
