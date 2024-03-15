#!/usr/bin/env python3

import io
import pynmea2
import serial
import re
from math import pi, sin, asin, cos, sqrt
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from geometry_msgs.msg import Quaternion

ser = serial.Serial("/dev/ttyACM0", 57600, timeout=0)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))


def dm_to_sd(dm):
    """
    Converts a geographic co-ordinate given in "degrees/minutes" dddmm.mmmm
    format (eg, "12319.943281" = 123 degrees, 19.943281 minutes)
    to a signed decimal (python float) format.
    """
    if not dm or dm == "0":
        return 0.0
    d, m = re.match(r"^(\d+)(\d\d\.\d+)$", dm).groups()
    return float(d) + float(m) / 60


def hav(rad):
    return sin(rad / 2) ** 2


def hav_dist(lat1, lat2, lon1, lon2):
    """
    Determines the distance between two points on a sphere given their longitudes and latitudes.
    """
    earth_radius = 6378  # km
    distance = (
        2
        * earth_radius
        * asin(sqrt(hav(lat2 - lat1) + cos(lat1) * cos(lat2) * hav(lon2 - lon1)))
    )
    return distance * 10


class GPSNode(Node):
    def __init__(self):
        super().__init__("gps_node")
        self.publisher_ = self.create_publisher(Quaternion, "gps_data", 10)
        self.counter = 1
        self.lat1 = self.lon1 = self.lat2 = self.lon2 = 0
        self.timer = self.create_timer(0.5, self.publish_gps_data)

    def publish_gps_data(self):
        try:
            line = sio.readline()
            if line.startswith("$GNGGA"):
                data = pynmea2.parse(line)
                lat, lon = float(dm_to_sd(data.lat)), float(dm_to_sd(data.lon))
                if data.lon_dir == "W":
                    lon = -lon
                if data.lat_dir == "S":
                    lat = -lat
                if self.counter == 1:
                    self.lat1, self.lon1 = lat, lon  # 1st measurement
                    self.counter += 1
                elif self.counter > 1:
                    self.lat2, self.lon2 = lat, lon
                    distance = hav_dist(
                        self.lat1, self.lat2, self.lon1, self.lon2
                    )  # 2nd measurement, continuous
                    print("distance:", distance)
                msg = Quaternion()
                msg.x, msg.y = lat, lon
                self.publisher_.publish(msg)

        except serial.SerialException as e:
            self.get_logger().error("Device error: {}".format(e))
        except pynmea2.ParseError as e:
            self.get_logger().error("Parse error: {}".format(e))
        except Exception as e:
            self.get_logger().error("Exception: {}".format(e))


def main(args=None):
    rclpy.init(args=args)
    gps_node = GPSNode()
    rclpy.spin(gps_node)
    gps_node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
