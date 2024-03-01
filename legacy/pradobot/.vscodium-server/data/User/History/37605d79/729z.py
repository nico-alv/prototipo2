import serial

ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)

while True:
    ser.write(64)
    ser.write(10)
    ser.write(13)