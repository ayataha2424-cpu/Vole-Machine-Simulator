#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>

class Instructions {
public:
    static void decode(uint16_t instruction, uint8_t& op, uint8_t& r, uint8_t& xy);
    static bool validate(uint16_t instruction);
};

#endif