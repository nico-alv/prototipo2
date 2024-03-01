#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
import pradobot_ultrasonic.ultrasonic_interface
from pradobot_ultrasonic.ultrasonic_interface import ultrasonic_distance

class ControlNode(Node):
    def __init__(self):
        super().__init__("control_node")
        self.encoder_publisher = self.create_publisher(Float32, 'pradobot/encoders',10)
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.get_logger().info("Control node initialized")

    def timer_callback(self):
        msg = Float32()
        distance = ultrasonic_distance()
        msg.data = distance
        self.encoder_publisher.publish(msg)
        self.get_logger().info('Publishing distance: "%s"' % msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = ControlNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
