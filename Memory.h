#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
using namespace std;

class Memory {
private:
	uint8_t memory[256];
public:
	static const uint8_t ScreenStart = 0xFF;
	Memory();
	uint8_t read(uint8_t address) const;
	void write(uint8_t address, uint8_t value);
	void clear();
	void displayMemory() const;
};
#endif