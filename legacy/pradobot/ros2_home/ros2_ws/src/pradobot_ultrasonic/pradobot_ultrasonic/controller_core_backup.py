#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy
import serial
import struct
import time


#Axis mapping
L_JOY_HORIZONTAL = 0
L_JOY_VERTICAL = 1
R_JOY_HORIZONTAL = 2
R_JOY_VERTICAL = 3
R_TRIGGER = 4
L_TRIGGER = 5
DPAD_HORIZONTAL = 6
DPAD_VERTICAL = 7

#Button mapping
L_BTN = 6
R_BTN = 8

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

class ControlNode(Node):
    active_mode = False
    high_speed_mode = False

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
        
        
        if(msg.axes[L_TRIGGER] != 1):
            if(not self.active_mode):
                self.active_mode = True
                self.get_logger().info("Motors activated.")
        else:
            self.active_mode = False
        
        if(msg.buttons[L_BTN] == 1):
            self.high_speed_mode = True
        else:
            self.high_speed_mode = False
        
        speed_multiplier = self.active_mode * (0.35 + 0.65*self.high_speed_mode)
        control_message = f"{msg.axes[L_JOY_HORIZONTAL]},{msg.axes[L_JOY_VERTICAL]*speed_multiplier}\n".encode("utf-8")
        ser.write(control_message)


        
        

def main(args=None):
    rclpy.init(args=args)
    node = ControlNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
