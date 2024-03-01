#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy
import serial
import struct
import time


DPAD_HORIZONTAL = 6
DPAD_VERTICAL = 7
R_TRIGGER = 4
L_TRIGGER = 5

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

class ControlNode(Node):
    previous_horizontal_selector_state = 0
    previous_vertical_selector_state = 0
    selected_motor = 0
    selected_motor_level = 0
    counter = 0
    def __init__(self):
        super().__init__("control_node")

        self.subscription = self.create_subscription(
            Joy,
            "/joy",
            self.listener_callback,
            10)
        self.subscription
        self.get_logger().info("Control node initialized")

    def listener_callback(self, msg):
        self.get_logger().info(f"DPAD: {msg.axes[DPAD_VERTICAL]}")
        if(msg.axes[DPAD_HORIZONTAL] != self.previous_horizontal_selector_state):
            self.previous_horizontal_selector_state = msg.axes[DPAD_HORIZONTAL]
            self.selected_motor -= msg.axes[DPAD_HORIZONTAL] #Using minus because right is -1 in this axis
            self.selected_motor %= 4
            self.selected_motor = int(self.selected_motor)
            if(msg.axes[DPAD_HORIZONTAL] != 0):
                self.get_logger().info(f'New Motor Selected: {self.selected_motor}')
        if(msg.axes[DPAD_VERTICAL] != self.previous_vertical_selector_state):
            self.previous_vertical_selector_state = msg.axes[DPAD_VERTICAL]
            self.selected_motor_level += msg.axes[DPAD_VERTICAL]
            self.selected_motor_level %= 100
            self.selected_motor_level = int(self.selected_motor_level)
            if(msg.axes[DPAD_HORIZONTAL] != 0):
                self.get_logger().info(f'New Motor Speed: {self.selected_motor_level}')


        if(msg.axes[R_TRIGGER] != 1):
            self.get_logger().info("BROOOM")
            
            data = [0, 0, 0, 0]
            data[self.selected_motor] = 10
            packed_data = struct.pack('iiii', *data)
            # Send the packed data
            # Send the string
            ser.write(packed_data)
        else:
            data = [0, 0, 0, 0]
            self.selected_motor_level = 0
            packed_data = struct.pack('iiii', *data)
            # Send the packed data
            # ser.write(packed_data)

        
        

def main(args=None):
    rclpy.init(args=args)
    node = ControlNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
