from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim',
            output='screen',
        ),
        Node(
            package='kal_d98',
            executable='mover_node',
            name='mover_node',
            output='screen',
        ),
        Node(
            package='kal_d98',
            executable='pose_logger_node',
            name='pose_logger_node',
            output='screen',
        ),
    ])