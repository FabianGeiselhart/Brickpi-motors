#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "bits.h"
#include "motors.h"
#include "uart.h"
#include <avr/interrupt.h>

/* Protocol:
 M/S: 2 7-bit bytes
 0. Number(Not yet)
 1. Destination (Not yet)

 2. Target (Motor 0 or 1)
 3. 4. Action (0 = Set speed, 1 = Set direction,
               2 = request encoder, (3 = Get config))
 5. Direction
 0..7. Speed (arbitrary if mode != 0)
 */

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
     UDR0 = dummy;                      // load xmt data in the register
     uart_motor(dummy);
}

void uart_motor(uint8_t data) {
    static bool speed = false;
    static uint8_t motor;
 
    if (speed) {
        motor_update(motor, motorDir(motor), data);
        speed = false;
        return;
    } else {
        motor = bit_is_set(data, 2);
    }

    if (!bit_is_set(data, 3) && !bit_is_set(data, 4)) { // Set Speed
        speed = true;
        uart_transmit('s');
    } else if (bit_is_set(data, 3) && !bit_is_set(data, 4)) { // Set direction
        motor_update(motor, bit_is_set(data, 5), motorSpeed(motor));
        uart_transmit('d');
    }
}

void uart_setup() {
    UBRR0H = (BAUDRATE >> 8);
    UBRR0L = BAUDRATE;

    // Set 8-Bit mode
    setBit(UCSR0C, UCSZ01);
    setBit(UCSR0C, UCSZ00);

    setBit(UCSR0B, RXEN0); // Receiver enable
    setBit(UCSR0B, TXEN0); // Transmitter enable
    
    setBit(UCSR0B, RXCIE0); // Enable RX Interrupt
    sei(); // Enable global interrupts
}
