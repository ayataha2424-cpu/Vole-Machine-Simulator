#include "Instructions.h"

void Instructions::decode(uint16_t instruction, uint8_t& op, uint8_t& r, uint8_t& xy) {
    op = (instruction >> 12) & 0x0F;
    r = (instruction >> 8) & 0x0F;
    xy = instruction & 0xFF;
}

bool Instructions::validate(uint16_t instruction) {
    uint8_t op = (instruction >> 12) & 0x0F;
    
    uint8_t d2 = (instruction >> 8) & 0x0F; 
    uint8_t d3 = (instruction >> 4) & 0x0F; 

    if (op < 1 || op > 0xC) {
        return false;
    }

    if (op == 0x4) {
        if (d2 != 0) {
            return false;
        }
    }
    else if (op == 0xA) {
        if (d3 != 0) {
            return false;
        }
    }
    else if (op == 0xC) {
        if (instruction != 0xC000) {
            return false;
        }
    }

    return true;
}
