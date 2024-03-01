import serial
import time

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

while True:
    ser.write("10,10,10,10".encode("utf-8"))
    time.sleep(0.01)