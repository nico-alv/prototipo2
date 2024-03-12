#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy
import serial
import struct
import time

L_JOY_HORIZONTAL = 0
L_JOY_VERTICAL = 1
R_JOY_HORIZONTAL = 2
R_JOY_VERTICAL = 3
R_TRIGGER = 4
L_TRIGGER = 5
DPAD_HORIZONTAL = 6
DPAD_VERTICAL = 7

L_BTN = 6
R_BTN = 8

ser = serial.Serial("/dev/ttyUSB0", 115200, rtscts=True)

class ControlNode(Node):
    active_mode, high_speed_mode = False, False

    def __init__(self):
        super().__init__("control_node")
        self.subscription = self.create_subscription(
            Joy, "/joy", self.listener_callback, 10
        )
        self.subscription
        self.get_logger().info("Control node initialized.")

    def listener_callback(self, msg):
        print(msg)
        if msg.axes[L_TRIGGER] != 1:
            if not self.active_mode:
                self.active_mode = True
                self.get_logger().info("Motors activated.")
        else:
            self.active_mode = False

        self.high_speed_mode = msg.buttons[L_BTN] == 1

        movement_x = msg.axes[L_JOY_HORIZONTAL] * self.active_mode
        movement_y = msg.axes[L_JOY_VERTICAL] * self.active_mode
        high_speed = int(self.high_speed_mode)

        control_message = f"{movement_x},{movement_y},{high_speed}\n".encode("utf-8")
        print(control_message)
        ser.write(control_message)

def main(args=None):
    rclpy.init(args=args)
    node = ControlNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()

