#include <avr/io.h>
#include "bits.h"
#include <stdbool.h>
#include "motors.h"

void motor_setup(uint8_t motor) {
    if(motor == 0) {
        DDRB |= 0x15; // 0b00010101; Port Setup
        setBit(PORTB, PORTB4);

        // Invert Mode
        setBit(TCCR1A, COM1B1);
        setBit(TCCR1A, COM1B0);

        // WGM Mode: 5; Fast PWM
        setBit(TCCR1A, WGM10);
        setBit(TCCR1B, WGM12);

        // Clock: clk/1024
        setBit(TCCR1B, CS10);
        setBit(TCCR1B, CS12);
    } else if(motor == 1) {
        DDRB |= 0x2A; //0b00101010; Port setup
        setBit(PORTB, PORTB5);

        // Invert mode
        setBit(TCCR2A, COM2A0);
        setBit(TCCR2A, COM2A1);

        // WGM Mode: 3, Fast PWM
        setBit(TCCR2A, WGM20);
        setBit(TCCR2A, WGM21);

        // Clock: clk/1024
        setBit(TCCR2B, CS20);
        setBit(TCCR2B, CS21);
        setBit(TCCR2B, CS22);

    }
}

void motor_update(uint8_t motor, bool direction, uint8_t speed) {
    if (motor == 0) { 
        if(direction) {
            setBit(PORTB, PORTB0);
            setBit(TCCR1A, COM1B0);
        } else {
            unsetBit(PORTB, PORTB0);
            unsetBit(TCCR1A, COM1B0);
        }
        OCR1B = speed;
    }
    else {
        if(direction) {
            setBit(PORTB, PORTB1);
        } else {
            speed = 255 - speed;
            unsetBit(PORTB, PORTB1);
        }
        OCR2A = speed;
    }
}

