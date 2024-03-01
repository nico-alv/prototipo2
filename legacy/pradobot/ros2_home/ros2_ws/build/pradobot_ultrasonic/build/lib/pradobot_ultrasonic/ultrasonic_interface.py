import Jetson.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
channel = 11 #Based on BCM pin numbering
def ultrasonic_distance():
   GPIO.setup(channel, GPIO.OUT)
   GPIO.output(channel, 0)
   time.sleep(0.000002)
   #send trigger signal
   GPIO.output(channel, 1)
   time.sleep(0.000002)
   GPIO.output(channel, 0)
   GPIO.setup(channel, GPIO.IN)
   max_count = 100
   count = 0
   while GPIO.input(channel) == 0 and count < max_count:
      count += 1
      pass
   start_time = time.time()
   while GPIO.input(channel) == 1:
      pass
   end_time = time.time()
   elapsed_time = end_time - start_time
   #print(f"The time elapsed was: {elapsed_time}")
   dist = (elapsed_time*34000)/2.0
   return dist

#while True:
#    print(ultrasonic_distance())
#    time.sleep(0.1)
