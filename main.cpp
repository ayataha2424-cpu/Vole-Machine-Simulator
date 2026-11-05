#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include "Registers.h"
#include "Memory.h" 
#include "Instructions.h"
#include "ProgramLoader.h"

using namespace std;

void displayMenu() {
    cout << "\n=========================================\n";
    cout << "        VOLE MACHINE SIMULATOR           \n";
    cout << "=========================================\n";
    cout << "1. Load a Program from File\n";
    cout << "2. Execute Next Step (Single Step)\n";
    cout << "3. Run Program Completely\n";
    cout << "4. Display Registers and Memory State\n";
    cout << "5. Exit\n";
    cout << "=========================================\n";
    cout << "Enter your choice (1-5): ";
}

void checkScreenOutput(Memory& mem) {
    uint8_t screenVal = mem.read(0xFF);
    if (screenVal != 0x00) {
        cout << "\n[Screen 0xFF Output Hex]: 0x" << hex << (int)screenVal << dec << endl;
    }
}

string byteToHexStr(uint8_t val) {
    char buf[3];
    sprintf(buf, "%02X", val);
    return string(buf);
}

bool stepExecution(Registers& reg, Memory& mem, int& pc, bool& isHalted) {
    if (isHalted) {
        cout << "\n[Info] Machine is currently halted. Reload a program to run again.\n";
        return false;
    }

    uint8_t b1 = mem.read(pc);
    uint8_t b2 = mem.read(pc + 1);

    string byte1 = byteToHexStr(b1);
    string byte2 = byteToHexStr(b2);

    string instruction = byte1 + byte2;

    char opcode = instruction[0];
    int r = stoi(instruction.substr(1, 1), nullptr, 16);
    int addressOrVal = stoi(instruction.substr(2, 2), nullptr, 16);

    cout << "\n[Executing] PC: 0x" << hex << pc
        << " | Instruction: " << instruction << dec << "\n";

    pc += 2;

    switch (opcode) {
    case '1': {
        uint8_t val = mem.read(addressOrVal);
        reg.setRegister(r, val);
        break;
    }
    case '2':
        reg.setRegister(r, addressOrVal);
        break;

    case '3': {
        string regStr = reg.getRegister(r);
        int regVal = stoi(regStr, nullptr, 16);
        mem.write(addressOrVal, (uint8_t)regVal);
        checkScreenOutput(mem);
        break;
    }
    case 'C':
    case 'c':
        isHalted = true;
        cout << "\n[Status] Machine Halted Successfully (Opcode C).\n";
        return false;

    default:
        cout << "\n[Error] Unsupported Opcode: " << opcode << "\n";
        break;
    }

    return true;
}

int main() {
    int choice;
    Registers reg;
    Memory mem;
    int pc = 0x00;
    bool isHalted = false;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            string filePath;
            cout << "Enter program file path: ";
            cin >> filePath;

            int startAddr;
            cout << "Enter starting memory address (e.g., 0): ";
            cin >> startAddr;

            ProgramLoader loader;
            vector<uint8_t> program = loader.loadProgram(filePath, startAddr);

            if (!program.empty()) {
                if (loader.loadIntoMemory(mem, program, startAddr)) {
                    pc = startAddr;
                    isHalted = false;
                    cout << "[Info] Program loaded and stored in memory successfully!\n";
                }
                else {
                    cout << "[Error] Failed to load program into memory.\n";
                }
            }
            else {
                cout << "[Error] Failed to read program file or file is empty.\n";
            }
            break;
        }
        case 2:
            stepExecution(reg, mem, pc, isHalted);
            break;
        case 3:
            cout << "[Info] Running the program completely...\n";
            while (!isHalted) {
                stepExecution(reg, mem, pc, isHalted);
            }
            break;
        case 4:
            cout << "[Info] Displaying registers and memory state...\n";
            mem.displayMemory();
            break;
        case 5:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "[Error] Invalid choice! Please select between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}