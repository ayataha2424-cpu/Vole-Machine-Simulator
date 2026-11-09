//====================================================================================
// Part 2
// STUDENT NAME: Aya Taha Esmail         ||| ID: 20251069 ||| section: S17, S18 ||| AI-Generated Designs, Comparison, Final Design
// STUDENT NAME: Ziad Ahmed Osman        ||| ID: 20251166 ||| section: S17, S18 ||| Introduction, Vole Machine Architecture, and Vole Programs
// STUDENT NAME: Habiba Essam Abo El Fotoh ||| ID: 20250187 ||| section: S15, S16 ||| Presentation Video, Report Compilation, and Formatting
// STUDENT NAME: Nada Said Ahmed         ||| ID: 20251453 ||| section: S17, S18 ||| Initial Design, Classes, and UML Diagram
//====================================================================================

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <limits>
#include "Registers.h"
#include "Memory.h" 
#include "Instructions.h"
#include "ProgramLoader.h"
#include "CPU.h"

using namespace std;

void displayMenu() {
    cout << "\n=========================================\n";
    cout << "        VOLE MACHINE SIMULATOR          \n";
    cout << "=========================================\n";
    cout << "1. Load and Run/Step Program\n";
    cout << "2. Overwrite a Register\n";
    cout << "3. Modify a Memory Cell\n";
    cout << "4. Display Memory and CPU Status\n";
    cout << "5. Exit\n";
    cout << "=========================================\n";
    cout << "Enter your choice (1-5): ";
}

int main() {
    int choice = 0;
    Memory mem;
    CPU cpu;
    ProgramLoader loader;

    do {
        displayMenu();
        if (!(cin >> choice)) {
            cout << "[Error] Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            string filePath;
            cout << "Enter program file path: ";
            cin >> filePath;

            int startAddr;
            cout << "Enter starting memory address in Hex (e.g., 00, 10, F0): 0x";
            cin >> hex >> startAddr;
            cin >> dec; 

            vector<uint8_t> program = loader.loadProgram(filePath, startAddr);

            if (!program.empty()) {
                if (loader.loadIntoMemory(mem, program, startAddr)) {
                    cpu.reset();
                    cpu.setPC(startAddr);
                    cout << "[Info] Program loaded and stored in memory successfully!\n";

                    cout << "\nChoose execution mode:\n";
                    cout << "1. Step-by-Step Execution\n";
                    cout << "2. Run Continuously until HALT\n";
                    cout << "Enter execution mode (1 or 2): ";
                    
                    int execMode;
                    cin >> execMode;

                    if (execMode == 1) {
                        cout << "\n--- Starting Step-by-Step Execution ---\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

                        while (!cpu.isHalted()) {
                            cpu.step(mem);
                            cpu.displayStatus();
                            mem.displayMemory();

                            cout << "\n[Press Enter to continue to the next step...]";
                            cin.get();
                        }
                        cout << "\n[Program Halted]\n";
                    } 
                    else if (execMode == 2) {
                        cout << "\n--- Running Program Continuously ---\n";
                        cpu.run(mem);
                        cpu.displayStatus();
                        mem.displayMemory();
                    }

                    if (!cpu.getScreenOutput().empty()) {
                        cout << "\n--- Final Screen Output (0xFF) ---\n" << cpu.getScreenOutput();
                    }
                }
                else {
                    cout << "[Error] Failed to load program into memory (address out of bounds).\n";
                }
            }
            else {
                cout << "[Error] Failed to read program file or file is empty.\n";
            }
            break;
        }
        case 2: {
            int regIndex;
            string hexVal;
            cout << "Enter register index (0-15): ";
            cin >> regIndex;
            cout << "Enter new 2-digit Hex value (e.g., A5): ";
            cin >> hexVal;

            if (regIndex >= 0 && regIndex < 16 && hexVal.length() == 2) {
                cpu.overwriteRegister(regIndex, hexVal);
                cout << "[Success] Register R" << uppercase << hex << regIndex << dec << " updated.\n";
            } else {
                cout << "[Error] Invalid register index or value format.\n";
            }
            break;
        }
        case 3: {
            int addr, val;
            cout << "Enter memory address in Hex (00-FF): 0x";
            cin >> hex >> addr;
            cout << "Enter value (0-255 in decimal): ";
            cin >> dec >> val;

            if (addr >= 0 && addr <= 255 && val >= 0 && val <= 255) {
                mem.write(static_cast<uint8_t>(addr), static_cast<uint8_t>(val));
                cout << "[Success] Memory cell 0x" << uppercase << hex << addr << dec << " updated.\n";
            } else {
                cout << "[Error] Invalid address or value range.\n";
            }
            break;
        }
        case 4:
            cout << "[Info] Displaying CPU status and memory state...\n";
            cpu.displayStatus();
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
