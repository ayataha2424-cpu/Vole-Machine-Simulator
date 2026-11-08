#include "ProgramLoader.h"
#include "Instructions.h" 
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

std::vector<uint8_t> ProgramLoader::loadProgram(const std::string& filename, int startAddress) {
    std::vector<uint8_t> program;

    if (!validateFileContent(filename)) {
        std::cerr << "Error: File content is invalid or contains malformed instructions.\n";
        return std::vector<uint8_t>();
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return std::vector<uint8_t>();
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        std::stringstream ss(line);
        std::string instructionStr;
        
        if (ss >> instructionStr) {
            if (instructionStr.length() == 4) {
                unsigned int value = 0;
                std::stringstream hexStream(instructionStr);
                hexStream >> std::hex >> value;
                
                if (!hexStream.fail()) {
                    uint16_t instVal = static_cast<uint16_t>(value);

                    if (!Instructions::validate(instVal)) {
                        std::cerr << "Error: Invalid Vole instruction detected -> 0x" 
                                  << std::uppercase << std::hex << instVal << "\n";
                        file.close();
                        return std::vector<uint8_t>();
                    }

                    program.push_back(static_cast<uint8_t>((instVal >> 8) & 0xFF));
                    program.push_back(static_cast<uint8_t>(instVal & 0xFF));
                }
            }
        }
    }
    file.close();

    if (!validateStartAddress(startAddress, program.size())) {
        std::cerr << "Error: Invalid start address or program size exceeds memory bounds.\n";
        return std::vector<uint8_t>();
    }

    return program;
}

bool ProgramLoader::validateFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line;
    bool hasValidInstruction = false;

    while (std::getline(file, line)) {
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        std::stringstream ss(line);
        std::string instruction;
        
        if (ss >> instruction) {
            if (instruction.length() != 4) {
                file.close();
                return false;
            }
            for (char c : instruction) {
                if (!std::isxdigit(static_cast<unsigned char>(c))) {
                    file.close();
                    return false;
                }
            }
            hasValidInstruction = true;
        }
    }
    
    file.close();
    return hasValidInstruction;
}

bool ProgramLoader::validateStartAddress(int startAddress, size_t programSize) {
    if (startAddress < 0 || startAddress > 254) {
        return false;
    }
    if (startAddress % 2 != 0) {
        return false;
    }
    if (programSize == 0 || startAddress + programSize > 256) {
        return false;
    }
    return true;
}

bool ProgramLoader::loadIntoMemory(Memory& memory, const std::vector<uint8_t>& program, int startAddress) {
    if (!validateStartAddress(startAddress, program.size())) {
        return false;
    }
    for (size_t i = 0; i < program.size(); ++i) {
        memory.write(static_cast<uint8_t>(startAddress + i), program[i]);
    }
    return true;
}
