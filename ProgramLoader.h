#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include <string>
#include <vector>
#include <cstdint>
#include "Memory.h"

class ProgramLoader {
private:
    std::string filename;
    int startAddress;

public:
    std::vector<uint8_t> loadProgram(const std::string& filename, int startAddress);
    bool validateFileContent(const std::string& filename);
    bool validateStartAddress(int startAddress, size_t programSize);
    bool loadIntoMemory(Memory& memory, const std::vector<uint8_t>& program, int startAddress);
};

#endif
