import serial

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

while True:
    ser.write(10)