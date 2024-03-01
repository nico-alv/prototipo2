import Jetson.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
channel = 29 #Based on BCM pin numbering
GPIO.setup(channel, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

while True:
    print(GPIO.input(channel))    
