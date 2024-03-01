import serial
import time

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)
count = -2
while True:
    ser.write(f"{count},{count}\n".encode("utf-8"))
    time.sleep(0.01)