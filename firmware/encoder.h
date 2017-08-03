#ifndef ENCODER_H_
#define ENCODER_H_

#define MASK 0x3C
#define MASK0 0x14
#define MASK1 0x28

extern volatile int32_t encoder_steps[2];
void encoder_setup();

#endif
