#!/usr/bin/env python
# coding: utf8

import serial
from struct import pack
from time import sleep
import RPi.GPIO as GPIO

# pack(">I", BYTE)

print("Imports abgeschlossen")

class BrickPi:

    def __init__(self):
        self.ser = serial.Serial("/dev/ttyAMA0")
        self.ser.baudrate = 9600

        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(24, GPIO.OUT)
        print("Start pressing reset")
        GPIO.output(24, GPIO.HIGH)
        sleep(.2)
        GPIO.output(24, GPIO.LOW)
        print("Stop pressing reset")

    def updateSpeed(self, motor, speed):
        tmp = 0
        tmp |= (motor << 2)
        self.ser.write(pack(">I", tmp))
        self.ser.write(pack(">I", speed))
        sleep(.1)

    def updateDirection(self, motor, direction):
        tmp = 0
        tmp |= (motor << 2)
        tmp |= (1 << 3)
        tmp |= (direction << 5)
        self.ser.write(pack(">I", tmp))
        sleep(.1)

    def stop():
        self.updateSpeed(0, 0)
        self.updateSpeed(1, 0)

if __name__ == "__main__":
    b = BrickPi()
    b.updateSpeed(1, 100)
    b.updateSpeed(0, 100)
    sleep(2)
    b.updateSpeed(0, 0)
    b.updateSpeed(1, 0)
