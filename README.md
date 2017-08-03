# Brickpi-motors

(WIP) A alternative firmware for the BrickPi+ to improve motor functionality.

## What it can / cannot

* You won't be able to use Lego sensors with this firmware.
(Maybe I'll add them eventually)

* At the moment you won't be able to use more than 2 motors.
(I'll add this feature probably very soon)

* I just provide a python library.

* This DOES NOT work on the BrickPi3

* The motors require less interaction between the BrickPi and
the Raspberry Pi. You send the BrickPi a command (eg. speed
on motor 1 to 100) and it will execute it until you send the
next one.

## Flashing the firmware

* Windows: There is no supported method and I won't change that.

* macOS: Install the avr toolchain and it should work. (I have not
tested it yet)

* Linux:
  1. Install the avr toolchain and GNU Make
  2. Clone the Repo
  3. Change into the firmware/ directory
  4. If you are not using a USBtinyISP programmer open firmware/makefile
and set $(PROGRAMMER) accordingly
  5. Connect your programmer to the ICSP port next to the GPIO Pins 
(You don't have to flash both chips b/c the firmware doesn't support that yet)
  6. Execute `make`. This should compile and flash the firmware

## Install the python library on your Raspberry Pi

1. Install python3 setuptools

2. Clone the Repo

3. Change into the library/ directory

4. Install the library
    sudo python3 setup.py install

## Usage

1. Import the library

        from BrickPiM import BrickPi

2. Initialise the BrickPI

        b = BrickPi(timeout=.1)

    * timeout: The time to wait for a reaction

3. Change the speed

        b.updateSpeed(motor, speed)

    * motor: MA = 0; MB = 1
    * Speed: any value between 0 and 255

4. Change the direction

        b.updateDirection(motor, direction)

    * motor: MA = 0; MB = 1
    * Direciton: 0 or 1

5. Close the connection and stop the motors
        b.stop()

