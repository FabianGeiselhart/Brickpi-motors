#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"
#include "motors.h"
#include "uart.h"
#include "bits.h"

volatile int32_t encoder_steps[2] = {0, 0};
volatile int32_t encoder_until[2] = {0, 0};
volatile uint8_t last_status;

void encoder_setup()
{
    // Configure Inputs and Pullups
    DDRD &= ~(1 << PIND2) & ~(1 << PIND3) &
            ~(1 << PIND4) & ~(1 << PIND5);
    PORTD |= (1 << PIND2) | (1 << PIND3) |
             (1 << PIND4) | (1 << PIND5);

    // Configure Interrupts
    PCICR |= (1 << PCIE2) | (1 << PCIE1);
    PCMSK2 |= (1 << PCINT20) | (1 << PCINT21);
    PCMSK1 |= (1 << PCINT18) | (1 << PCINT19);

    last_status = PIND & MASK;
}

// Check whether to stop the motors due to an encoder command from the RPi
void encoder_check(uint8_t motor) {
    // Value of enc before moveSteps() command was received 
    static int32_t encoder_last[2] = {0, 0}; 

    if (encoder_until[motor] != 0) { // Encoder operation pending
        if (encoder_last[motor] == 0) { // First run
            encoder_last[motor] = encoder_steps[motor];
        }
        // Motor reached wanted state
        if (encoder_last[motor] + encoder_until[motor] <= encoder_steps[motor] ||
                encoder_last[motor] - encoder_until[motor] >= encoder_steps[motor]) {
            motor_update(motor, motorDir(motor), 0);
            encoder_last[motor] = 0;
            encoder_until[motor] = 0;
            uart_transmit('0');
        }
    }
}

// Update encoder_steps[motor] accorgingly
ISR(PCINT2_vect)
{
    uint8_t status = PIND;
    uint8_t PinA, PinB, encoder;
    status &= MASK;

    // Check whick encoder triggerd interrupt
    if ((status & MASK0) != (last_status & MASK0)) {
        PinA = PIND2; PinB = PIND4;
        encoder = 0;
    } else {
        PinA = PIND3; PinB = PIND5;
        encoder = 1;
    }

    // Evaluate Encoder
    if (checkBit(status, PinA) == checkBit(status, PinB)) {
        encoder_steps[encoder]++;
    } else {
        encoder_steps[encoder]--;
    }

    last_status = status;
}


