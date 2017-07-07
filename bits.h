#ifndef BITS_H_
#define BITS_H_

#define setBit(reg, bit) (reg) |= (1 << (bit))
#define unsetBit(reg, port) (reg) &= ~(1 << (port))

#endif
