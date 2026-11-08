#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include "Registers.h"
#include "Memory.h" 
#include "Instructions.h"
#include "ProgramLoader.h"
#include "CPU.h"

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

int main() {
    int choice;
    Registers reg;
    Memory mem;
    CPU cpu;

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
                bool validProgram = true;
                for (size_t i = 0; i < program.size(); i += 2) {
                    if (i + 1 < program.size()) {
                        char op = toupper(program[i] >> 4); // Check high nibble if stored byte-wise or string format depending on your loader
                    }
                }

                if (loader.loadIntoMemory(mem, program, startAddr)) {
                    cpu.setPC(startAddr);
                    cpu.setHalted(false);
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
            cpu.step(reg, mem);
            break;
        case 3:
            cpu.run(reg, mem);
            break;
        case 4:
            cout << "[Info] Displaying memory state...\n";
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
