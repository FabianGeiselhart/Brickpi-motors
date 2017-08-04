#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include "bits.h"
#include "motors.h"
#include "uart.h"
#include "encoder.h"

int main(void)
{

    motor_setup(0);
    motor_setup(1);

    uart_setup();
    uart_transmit('i');
 
    encoder_setup();
    sei();

    for ( ; ; ) {
        for (int i = 0; i < 2; i++) {
            encoder_check(i);
        }
    } 

    return(0);
}

