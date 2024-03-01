import serial

ser = serial.Serial('/dev/ttyTHS0', 9600, timeout=0, rtscts=True)

while True:
    byte = ser.read(1)
    print(byte)