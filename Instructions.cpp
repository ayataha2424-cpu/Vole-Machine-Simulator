#include "Instructions.h"
void Instructions::decode(uint16_t instruction, uint8_t& op, uint8_t& r, uint8_t& xy) {
    op = (instruction >> 12) & 0x0F; 
    r = (instruction >> 8) & 0x0F;  
    xy = instruction & 0xFF;        
}
bool Instructions::validate(uint16_t instruction) {
    uint8_t op = (instruction >> 12) & 0x0F;
    if (op >= 1 && op <= 0xC) {
        return true;
    }
    return false;
}