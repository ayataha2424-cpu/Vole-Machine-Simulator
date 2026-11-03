#include "Registers.h"
Registers::Registers() {
    for (int i = 0; i < 16; i++) {
        reg[i] = 0;
    }
}
uint8_t Registers::getValue(int i) {
    if (i >= 0 && i < 16) {
        return reg[i];
    }
    return 0; 
}
void Registers::setValue(int i, uint8_t val) {
    if (i >= 0 && i < 16) {
        reg[i] = val;
    }
}
void Registers::display() {
    std::cout << "--- Registers State ---\n";
    for (int i = 0; i < 16; i++) {
        std::cout << "R" << i << ": " << (int)reg[i] << "  ";
        if ((i + 1) % 4 == 0) std::cout << "\n";
    }
}