#!/usr/bin/env pytohn3
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import pradobot_ultrasonic.joystick_interface

class JoystickNode(Node):
    def __init__(self):
        super().__init__("joystick_node")
        self.cmd_dist_pub_ = self.create_publisher(String, 'pradobot/joystick',10)
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.get_logger().info("Joystick node initialized.")

    def timer_callback(self):
        msg = String()
        msg.data = 'Hello World'
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = JoystickNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
