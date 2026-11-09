#ifndef ALU_H
#define ALU_H

#include <cstdint>

class ALU {
    private:
    uint8_t result;

    public:
    uint8_t addInteger(uint8_t a , uint8_t b);
    uint8_t addFloat(uint8_t a , uint8_t b);
    uint8_t bitwiseAnd(uint8_t a , uint8_t b);
    uint8_t bitwiseOr(uint8_t a , uint8_t b);
    uint8_t bitwiseXor(uint8_t a , uint8_t b);
    uint8_t rotateRight(uint8_t value , uint8_t steps);
};


#endif //ALU_H
