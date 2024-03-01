from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='pradobot_ultrasonic',
            namespace='pradobot_control',
            executable='control_node',
            name='robot_controller'
        ),
        Node(
            package='joystick',
            namespace='pradobot_control',
            executable='joy_node',
            name='joystick_node'
        )
    ])