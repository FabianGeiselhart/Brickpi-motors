#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "bits.h"
#include "motors.h"
#include "uart.h"

int main(void)
{
    motor_setup(0);
    motor_setup(1);

    uart_setup();
    uart_transmit('i');

    for ( ; ; ) {
    } 

    return(0);
}
