#include <iostream>
#include <vector>
#include "Registers.h"
#include "Memory.h" 
#include "Instructions.h"

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
void checkScreenOutput(const unsigned char memory[]) {
    if (memory[0xFF] != 0) {
        cout << "\n[Screen 0xFF Output]: " << memory[0xFF] << endl;
    }
}

int main() {
    int choice;
    unsigned char memory[256] = { 0 }; 
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "[Info] Loading program from file...\n";
            break;
        case 2:
            cout << "[Info] Executing a single step...\n";
            checkScreenOutput(memory);
            break;
        case 3:
            cout << "[Info] Running the program completely...\n";
            checkScreenOutput(memory);
            break;
        case 4:
            cout << "[Info] Displaying registers and memory state...\n";
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