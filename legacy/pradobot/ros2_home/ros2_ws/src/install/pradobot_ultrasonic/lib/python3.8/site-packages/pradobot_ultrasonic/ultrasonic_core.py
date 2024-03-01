#!/usr/bin/env pytohn3
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import pradobot_ultrasonic.ultrasonic_interface

class UltrasonicNode(Node):
    def __init__(self):
        super().__init__("ultrasonic_node")
        self.cmd_dist_pub_ = self.create_publisher(String, 'pradobot/ultrasonic',10)
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.get_logger().info("Ultrasonic initialized")

    def timer_callback(self):
        msg = String()
        distance = ultrasonic_interface.distance()
        self.get_logger().info(f"Distance: {distance}")
        msg.data = 'Hello World'
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = UltrasonicNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
