# ROS 2 Autonomous Perception Pipeline

A ROS 2 C++ perception package implementing managed lifecycle nodes (`rclcpp_lifecycle`) to process 2D LiDAR scans (`sensor_msgs/msg/LaserScan`) and publish filtered 3D visualization markers (`visualization_msgs/msg/MarkerArray`).

## Features
- **Lifecycle Node Architecture**: Managed state transitions (`on_configure`, `on_activate`, `on_deactivate`).
- **QoS & Callback Management**: Custom execution callback groups paired with `SensorDataQoS` for real-time sensor streams.
- **Obstacle Detection**: Real-time laser range filtering and point translation to 3D spatial markers.

## Build & Run
