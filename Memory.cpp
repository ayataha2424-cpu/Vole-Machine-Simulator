#include "Memory.h"
#include <iostream>
#include <iomanip>
#include <ios>

Memory::Memory() {
    clear();
}

uint8_t Memory::read(uint8_t address) const {
    return memory[address];
}

void Memory::write(uint8_t address, uint8_t value) {
    memory[address] = value;
}

void Memory::clear() {
    for (int i = 0; i < 256; ++i) {
        memory[i] = 0;
    }
}

void Memory::displayMemory() const {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    
    std::cout << "\n--- Memory State (16x16 Matrix) ---\n";
    
    std::cout << "   ";
    for (int col = 0; col < 16; ++col) {
        std::cout << " " << std::uppercase << std::hex << std::setw(1) << col;
    }
    std::cout << "\n  +-----------------------------------\n";
    for (int row = 0; row < 16; ++row) {
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (row * 16) << " | ";
        
        for (int col = 0; col < 16; ++col) {
            int address = (row * 16) + col;
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(memory[address]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "--------------------------------------\n";

    std::cout.copyfmt(oldState);
}
