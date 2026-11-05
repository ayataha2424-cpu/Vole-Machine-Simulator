#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H
#include <string>
#include <cstdint>
#include <vector>
#include "Memory.h"

using namespace std;
class ProgramLoader {
private:
	string filename;
	int startAddress;
public:
	vector<uint8_t> loadProgram(const string& filename, int startAddress);
	bool validateFileName(const string& filename);
	bool validateStartAddress(int startAddress, size_t programSize);
	bool loadIntoMemory(Memory& memory, const vector<uint8_t>& program, int startAddress);
};

#endif