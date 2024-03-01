import serial

ser = serial.Serial('/dev/ttyTHS0', 115200, timeout=0, rtscts=True)

while True:
    byte = ser.read(1)
    print(byte)