from gpiozero import PWMLED
from time import sleep
from math import sin

led = PWMLED(2)

while True:
    t = time.now()
    led.value = (sin(t) + 1) / 2
    sleep(0.01)
