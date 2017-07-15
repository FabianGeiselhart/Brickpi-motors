#include <avr/io.h>
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
    
    GPIOR0 = 0;

    int last = GPIOR0;

    for ( ; ; ) {
        /* TODO: Fix this
         * This is a ugly workaround, b/c the ISR keeps 
         * repeating itself 4x. I dunno why.
         */
        if (GPIOR0 != last && GPIOR0 % 4 == 0) {
            uart_motor(GPIOR1);
            last += 4;
        }
    } 

    return(0);
}
