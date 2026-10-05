#include "ros2_autonomous_perception_pipeline/obstacle_filter_node.hpp"
#include <cmath>

namespace perception
{

ObstacleFilterNode::ObstacleFilterNode(const rclcpp::NodeOptions & options)
: rclcpp_lifecycle::LifecycleNode("obstacle_filter_node", options)
{
  this->declare_parameter<double>("distance_threshold", 1.5);
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ObstacleFilterNode::on_configure(const rclcpp_lifecycle::State &)
{
  this->get_parameter("distance_threshold", distance_threshold_);
  cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
  
  auto sub_options = rclcpp::SubscriptionOptions();
  sub_options.callback_group = cb_group_;

  scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
    "/scan", rclcpp::SensorDataQoS(),
    std::bind(&ObstacleFilterNode::scanCallback, this, std::placeholders::_1),
    sub_options);

  marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(
    "/filtered_obstacles", rclcpp::SystemDefaultsQoS());

  RCLCPP_INFO(this->get_logger(), "Node Configured. Threshold: %.2fm", distance_threshold_);
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ObstacleFilterNode::on_activate(const rclcpp_lifecycle::State & state)
{
  LifecycleNode::on_activate(state);
  marker_pub_->on_activate();
  RCLCPP_INFO(this->get_logger(), "Node Activated.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ObstacleFilterNode::on_deactivate(const rclcpp_lifecycle::State & state)
{
  LifecycleNode::on_deactivate(state);
  marker_pub_->on_deactivate();
  RCLCPP_INFO(this->get_logger(), "Node Deactivated.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

void ObstacleFilterNode::scanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
  if (!marker_pub_->is_activated()) return;
  
  visualization_msgs::msg::MarkerArray markers;
  int id = 0;

  for (size_t i = 0; i < msg->ranges.size(); ++i) {
    float range = msg->ranges[i];
    if (range < distance_threshold_ && range > msg->range_min) {
      visualization_msgs::msg::Marker marker;
      marker.header = msg->header;
      marker.id = id++;
      marker.type = visualization_msgs::msg::Marker::SPHERE;
      marker.scale.x = 0.2; marker.scale.y = 0.2; marker.scale.z = 0.2;
      marker.color.r = 1.0f; marker.color.a = 1.0f;
      
      double angle = msg->angle_min + i * msg->angle_increment;
      marker.pose.position.x = range * std::cos(angle);
      marker.pose.position.y = range * std::sin(angle);

      markers.markers.push_back(marker);
    }
  }
  marker_pub_->publish(markers);
}

}  // namespace perception
