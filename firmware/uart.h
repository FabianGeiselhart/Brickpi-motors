#ifndef UART_H_
#define UART_H_

#define BAUD 9600// define baud
#define BAUDRATE ((F_CPU)/(BAUD*16)-1) // set baud rate value for UBRR

void uart_transmit(unsigned char data);
void uart_motor(uint8_t data);
void uart_setup();

#endif
