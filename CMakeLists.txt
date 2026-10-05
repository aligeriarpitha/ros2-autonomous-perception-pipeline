cmake_minimum_required(VERSION 3.8)
project(ros2_autonomous_perception_pipeline)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)

include_directories(include)

add_executable(perception_node src/obstacle_filter_node.cpp)
ament_target_dependencies(perception_node
  rclcpp
  rclcpp_lifecycle
  sensor_msgs
  visualization_msgs
)

install(TARGETS perception_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
