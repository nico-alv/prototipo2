import serial
import time

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

while True:
    ser.write("Hola\n".encode("utf-8"))