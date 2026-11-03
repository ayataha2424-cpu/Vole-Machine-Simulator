#include "ProgramLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

vector<uint8_t> ProgramLoader::loadProgram(const string& filename, int startAddress) {
	vector<uint8_t> program;
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Error: Could not open file " << filename << endl;
		return program;
	}
	string line;
	while (getline(file, line)) {
		size_t commentPos = line.find(';');
		if (commentPos != string::npos) {
			line = line.substr(0, commentPos);
		}
		stringstream ss(line);
		string instruction;
		while (ss >> instruction) {
			unsigned int value;
			stringstream hexStream(instruction);
			hexStream >> hex >> value;
			if (!hexStream.fail() && hexStream.eof()) {
				program.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
				program.push_back(static_cast<uint8_t>(value & 0xFF));
			}
		}
	}
	file.close();
	return program;
}

bool ProgramLoader::validateFileName(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error: Could not open file " << filename << endl;
		return false;
	}
	string line;
	bool hasValidInstruction = false;
	while (getline(file, line)) {
		size_t commentPos = line.find(';');
		if (commentPos != string::npos) {
			line = line.substr(0, commentPos);
		}
		stringstream ss(line);
		string instruction;
		while (ss >> instruction) {
			if (instruction.length() != 4) {
				file.close();
				return false;
			}
			for (char c : instruction) {
				if (!isxdigit(static_cast<unsigned char>(c))) {
					file.close();
					return false;
				}
			}
			hasValidInstruction = true;
		}
		file.close();
		return hasValidInstruction;
	}
}

bool ProgramLoader::validateStartAddress(int startAddress, size_t programSize) {
	if (startAddress < 0 || startAddress > 254) {
		return false;
	}
	if (startAddress % 2 != 0) {
		return false;
	}
	if (programSize == 0 || startAddress + programSize > 255) {
		return false;
	}
	return true;
}

bool ProgramLoader::loadIntoMemory(Memory& memory, const vector<uint8_t>& program, int startAddress) {
	if (!validateStartAddress(startAddress, program.size())) {
		return false;
	}
	for (size_t i = 0; i < program.size(); ++i) {
		memory.write(startAddress + i, program[i]);
	}
	return true;
}