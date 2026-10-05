from launch import LaunchDescription
from launch_ros.actions import LifecycleNode

def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(
            package='ros2_autonomous_perception_pipeline',
            executable='perception_node',
            name='perception_node',
            namespace='',
            output='screen',
            parameters=[{
                'min_range': 0.2,
                'max_range': 10.0,
            }]
        )
    ])
