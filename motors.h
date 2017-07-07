#ifndef MOTORS_H_
#define MOTORS_H_
#include <stdbool.h>

void motor_setup(uint8_t motor);
void motor_update(uint8_t motor, bool direction, uint8_t speed);

#endif
