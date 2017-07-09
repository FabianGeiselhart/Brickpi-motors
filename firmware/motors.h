#ifndef MOTORS_H_
#define MOTORS_H_
#include <stdbool.h>

#define motorSpeed(motor) (motor == 0) ? OCR1B : OCR2A
#define motorDir(motor) (motor == 0) ? PORTB1 : PORTB0

void motor_setup(uint8_t motor);
void motor_update(uint8_t motor, bool direction, uint8_t speed);

#endif
