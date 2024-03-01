#!/usr/bin/env pytohn3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
import pradobot_ultrasonic.lidar_interface
from pradobot_ultrasonic.lidar_interface import LidarManager

class LidarNode(Node):
    def __init__(self):
        super().__init__("lidar_node")
        self.cmd_dist_pub_ = self.create_publisher(Float32, 'pradobot/lidar',10)
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.lidar_manager = LidarManager('/dev/ttyACM0')
        self.get_logger().info("LiDAR initialized")

    def timer_callback(self):
        msg = Float32()
        distance = ultrasonic_distance()
        msg.data = distance
        self.cmd_dist_pub_.publish(msg)
        self.get_logger().info('Publishing distance: "%s"' % msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = LidarNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
