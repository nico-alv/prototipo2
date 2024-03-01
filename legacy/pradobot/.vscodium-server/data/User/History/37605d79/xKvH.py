import serial
import time

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

while True:
    byte = ser.read(1)
    print(byte)
    ser.write(64)
    ser.write(10)
    ser.write(13)
    time.sleep(0.1)