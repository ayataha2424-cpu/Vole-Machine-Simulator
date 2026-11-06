#ifndef ALU_H
#define ALU_H

#include <cstdint>

class ALU {
    private:
    uint8_t result;

    public:
    uint8_t addinteger(uint8_t a , uint8_t b);
    uint8_t addfloating(uint8_t a , uint8_t b);
    uint8_t bitwiseand(uint8_t a , uint8_t b);
    uint8_t bitwiseor(uint8_t a , uint8_t b);
    uint8_t bitwisexor(uint8_t a , uint8_t b);
    uint8_t rotateright(uint8_t value , uint8_t steps);
};


#endif //ALU_H
