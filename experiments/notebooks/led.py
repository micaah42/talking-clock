import RPi.GPIO as GPIO
import math
import time

PIN = 37

GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN, GPIO.OUT)

t = time.perf_counter()

p = GPIO.PWM(PIN, 550)
p.start(0)

while True:
    p.ChangeDutyCycle(50 * (math.sin(1e-4 * t) + 1))
