#!/usr/bin/env python
# coding: utf8

import serial
from struct import pack
from time import sleep
import RPi.GPIO as GPIO


class BrickPi:

    def __init__(self, timeout=.1):
        self.ser = serial.Serial("/dev/ttyAMA0")
        self.ser.baudrate = 9600
        self.ser.timeout = timeout

    def updateSpeed(self, motor, speed):
        """ Update speed of given motor"""
        tmp = 0
        tmp |= (motor << 2)

        self.ser.flushInput()
        self.ser.write(pack("!B", tmp))
        self.ser.write(pack("!B", speed))
        if self.ser.read() != b's':
            raise RuntimeError('The BrickPi is not responding')

    def updateDirection(self, motor, direction):
        """ Update Direction of given motor """
        tmp = 0
        tmp |= (motor << 2)
        tmp |= (1 << 3)
        tmp |= (direction << 5)

        self.ser.flushInput()
        self.ser.write(pack("!B", tmp))
        if self.ser.read() != b'd':
            raise RuntimeError('The BrickPi is not responding')

    def moveSteps(self, motor, steps, interrupt=True):
        """ Keep motor running until encoder-value
            changed by steps """
        tmp = 0
        tmp |= (motor << 2)
        tmp |= (1 << 3) | (1 << 4)

        self.ser.flushInput()
        self.ser.write(pack("!B", tmp))
        self.ser.write(pack(">i", steps))
        if self.ser.read() != b'e':
            raise RuntimeError('The BrickPi is not responding')
        if interrupt:
            while self.ser.read() != b'0':
                pass

    def stop(self):
        self.updateSpeed(0, 0)
        self.updateSpeed(1, 0)
        self.updateDirection(0, 0)
        self.updateDirection(1, 0)
        self.ser.stop()
