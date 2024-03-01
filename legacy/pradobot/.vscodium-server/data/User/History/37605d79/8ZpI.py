import serial
import time

# ser = serial.Serial('/dev/ttyTHS0', 115200, rtscts=True)
count = -1

high_Speed = True
active = False

speed = active * (0.35 + 0.65 * high_Speed)
print(speed)

# while True:
#     ser.write(f"{count},{count}\n".encode("utf-8"))
#     time.sleep(0.01)
    