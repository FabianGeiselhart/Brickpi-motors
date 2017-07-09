#ifndef BITS_H_
#define BITS_H_

#define setBit(reg, bit) (reg) |= (1 << (bit))
#define unsetBit(reg, bit) (reg) &= ~(1 << (bit))
#define checkBit(reg, bit) (((reg) & (1 << (bit))) != 0) ? true : false

#endif
