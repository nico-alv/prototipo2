import serial

ser = serial.Serial('/dev/ttyTHS0', 115200)

while True:
    byte = ser.read(1)
    print(byte)