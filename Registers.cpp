#include "Registers.h"

Registers::Registers() {
    for (int i = 0; i < 16; ++i) {
        registers[i] = "00";
    }
}

void Registers::setRegister(int index, std::string val) {
    if (index >= 0 && index < 16) {
        registers[index] = val;
    }
}

void Registers::setRegister(int index, int val) {
    if (index >= 0 && index < 16) {
        char buf[3];
        sprintf(buf, "%02X", val);
        registers[index] = std::string(buf);
    }
}

std::string Registers::getRegister(int index) const {
    if (index >= 0 && index < 16) {
        return registers[index];
    }
    return "00";
}

void Registers::displayRegisters() const {
    std::cout << "\n--- Registers State ---\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << "R" << std::hex << i << ": " << registers[i] << "\n";
    }
    std::cout << "-----------------------\n";
}