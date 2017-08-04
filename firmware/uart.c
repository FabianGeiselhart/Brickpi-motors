#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "bits.h"
#include "motors.h"
#include "uart.h"
#include "encoder.h"
#include <avr/interrupt.h>

/* Protocol:
 M/S: 1-5 bytes
 0. Number(Not yet)
 1. Destination (Not yet)

 2. Target (Motor 0 or 1)
 3. 4. Action (0 = Set speed, 1 = Set direction,
               2 = rotate encoder)
 5. Direction
 
 if mode == 0: Second bit = speed
 if mode == 2: Next 4 Bits = Encoder steps

 */

volatile int32_t encoder_until[2];

void uart_setup() {
    UBRR0H = (BAUDRATE >> 8);
    UBRR0L = BAUDRATE;

    // Set 8-Bit mode
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); //Enable USART RX + TX + RX Interupt
}

// function to send data
void uart_transmit(unsigned char data)
{
    while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
    UDR0 = data;                                   // load data in the register
}

ISR(USART_RX_vect) { // React on sent data
     unsigned char dummy;
     dummy = UDR0;                     // dump the rcvr buffer
     while (!( UCSR0A & (1<<UDRE0)));  // wait until the register is free
     uart_motor(dummy);
}

// Evaluate received data
void uart_motor(uint8_t data) {
    // 0 bit is speed mode, 1 bit is encoder mode
    static bool state_speed = false;
    static bool state_enc = false;
    static uint8_t motor;
    static uint8_t counter = 0;
    static int32_t tmp = 0;
 
    if (state_speed) { // Data is speed
        motor_update(motor, motorDir(motor), data);
        state_speed = false;
        uart_transmit('s');
        return;
    } else if (state_enc){ // Data is part of encoder_until
        tmp = (tmp << 8) | data;       
        counter++;
        if (counter == 4) {
            state_enc = false;
            encoder_until[motor] = tmp;
            tmp = 0; counter = 0;
            uart_transmit('e');
        }
        return;
    } else {
        motor = checkBit(data, 2);
    }

    if (!bit_is_set(data, 3) && !bit_is_set(data, 4)) { // Set Speed
        state_speed = true;
    } else if (bit_is_set(data, 3) && !bit_is_set(data, 4)) { // Set direction
        motor_update(motor, bit_is_set(data, 5), motorSpeed(motor));
        uart_transmit('d');
    } else if (bit_is_set(data, 3) && bit_is_set(data, 4)) { // Encoder steps
        state_enc = true;
    }
}

