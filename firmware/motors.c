#include <avr/io.h>
#include <stdbool.h>
#include "bits.h"
#include "motors.h"

void motor_setup(uint8_t motor) {
    if(motor == 0) {
        DDRB |= 0x15; // 0b00010101; Port Setup
        PORTB |= (1 << PORTB4);
        TCCR1A |= (1 << COM1B1) | (1 << COM1B0); // Invert Mode
        TCCR1B |= (1 << CS10) | (1 << CS12); // Clock: clk/1024

        // WGM Mode: 5; Fast PWM
        TCCR1A |= (1 << WGM10);
        TCCR1B |= (1 << WGM12);
    } else if(motor == 1) {
        DDRB |= 0x2A; //0b00101010; Port setup
        PORTB |= (1 << PORTB5);
        TCCR2A |= (1 << COM2A1); // Invert mode
        TCCR2A |= (1 << WGM20) | (1 << WGM21); // WGM Mode: 3, Fast PWM
        TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22); // Clock: clk/1024
    }
        motor_update(motor, false, 0);
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

        if (speed == 0) {
            unsetBit(PORTB, PORTB4);
        } else {
            setBit(PORTB, PORTB4);
        }
    }
    else {
        if(direction) {
            setBit(PORTB, PORTB1);
            setBit(TCCR2A, COM1A0);
        } else {
            unsetBit(PORTB, PORTB1);
            unsetBit(TCCR2A, COM1A0);
        }
        OCR2A = speed;

        if (speed == 0) {
            unsetBit(PORTB, PORTB5);
        } else {
            setBit(PORTB, PORTB5);
        }
    }
}

