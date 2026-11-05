#include "Memory.h"
#include <iostream>
#include <iomanip>

Memory::Memory() {
	for (int i = 0; i < 256; ++i) {
		memory[i] = 0;
	}
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
	ios oldState(nullptr);
	oldState.copyfmt(cout);
	cout << hex << uppercase << setfill('0');
	for (int i = 0; i < 256; ++i) {
		cout << "Address: 0x" << setw(2) << static_cast<int>(i) << endl
			<< " Value: 0x" << setw(2) << static_cast<int>(memory[i]) << endl;
	}
	cout.copyfmt(oldState);
}