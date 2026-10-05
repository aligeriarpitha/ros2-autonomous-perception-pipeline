#include "ros2_autonomous_perception_pipeline/obstacle_filter_node.hpp"

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

ObstacleFilterNode::ObstacleFilterNode(const rclcpp::NodeOptions & options)
: rclcpp_lifecycle::LifecycleNode("obstacle_filter_node", options)
{
  declare_parameter("min_range", 0.2);
  declare_parameter("max_range", 10.0);
}

CallbackReturn ObstacleFilterNode::on_configure(const rclcpp_lifecycle::State &)
{
  min_range_ = get_parameter("min_range").as_double();
  max_range_ = get_parameter("max_range").as_double();

  scan_sub_ = create_subscription<sensor_msgs::msg::LaserScan>(
    "scan", rclcpp::SensorDataQoS(),
    std::bind(&ObstacleFilterNode::scan_callback, this, std::placeholders::_1));

  marker_pub_ = create_publisher<visualization_msgs::msg::MarkerArray>("detected_obstacles", 10);

  RCLCPP_INFO(get_logger(), "ObstacleFilterNode configured successfully.");
  return CallbackReturn::SUCCESS;
}

CallbackReturn ObstacleFilterNode::on_activate(const rclcpp_lifecycle::State & state)
{
  rclcpp_lifecycle::LifecycleNode::on_activate(state);
  RCLCPP_INFO(get_logger(), "ObstacleFilterNode activated.");
  return CallbackReturn::SUCCESS;
}

CallbackReturn ObstacleFilterNode::on_deactivate(const rclcpp_lifecycle::State & state)
{
  rclcpp_lifecycle::LifecycleNode::on_deactivate(state);
  RCLCPP_INFO(get_logger(), "ObstacleFilterNode deactivated.");
  return CallbackReturn::SUCCESS;
}

CallbackReturn ObstacleFilterNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  scan_sub_.reset();
  marker_pub_.reset();
  RCLCPP_INFO(get_logger(), "ObstacleFilterNode cleaned up.");
  return CallbackReturn::SUCCESS;
}

CallbackReturn ObstacleFilterNode::on_shutdown(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(get_logger(), "ObstacleFilterNode shutting down.");
  return CallbackReturn::SUCCESS;
}

void ObstacleFilterNode::scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
  if (!get_current_state().label().compare("active")) {
    return;
  }

  visualization_msgs::msg::MarkerArray marker_array;
  int id = 0;

  for (size_t i = 0; i < msg->ranges.size(); ++i) {
    float range = msg->ranges[i];
    if (range >= min_range_ && range <= max_range_) {
      double angle = msg->angle_min + i * msg->angle_increment;
      visualization_msgs::msg::Marker marker;
      marker.header.frame_id = msg->header.frame_id;
      marker.header.stamp = now();
      marker.ns = "obstacles";
      marker.id = id++;
      marker.type = visualization_msgs::msg::Marker::SPHERE;
      marker.action = visualization_msgs::msg::Marker::ADD;
      marker.pose.position.x = range * std::cos(angle);
      marker.pose.position.y = range * std::sin(angle);
      marker.pose.position.z = 0.0;
      marker.scale.x = 0.1;
      marker.scale.y = 0.1;
      marker.scale.z = 0.1;
      marker.color.a = 1.0;
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      marker_array.markers.push_back(marker);
    }
  }

  if (marker_pub_) {
    marker_pub_->publish(marker_array);
  }
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ObstacleFilterNode>(rclcpp::NodeOptions());
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}
