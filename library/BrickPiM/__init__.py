#!/usr/bin/env python
# coding: utf8

import serial
from struct import pack
from time import sleep
import RPi.GPIO as GPIO


class BrickPi:

    def __init__(self):
        self.ser = serial.Serial("/dev/ttyAMA0")
        self.ser.baudrate = 9600

    def updateSpeed(self, motor, speed):
        tmp = 0
        tmp |= (motor << 2)
        self.ser.write(pack("!B", tmp))
        self.ser.write(pack("!B", speed))

    def updateDirection(self, motor, direction):
        tmp = 0
        tmp |= (motor << 2)
        tmp |= (1 << 3)
        tmp |= (direction << 5)
        self.ser.write(pack("!B", tmp))

    def stop(self):
        self.updateSpeed(0, 0)
        self.updateSpeed(1, 0)
