#ifndef CPU_H
#define CPU_H

#include <string>
#include <iostream>
#include <vector>
#include "Memory.h"
#include "Registers.h"
#include "ALU.h"
#include "Instructions.h"

class CPU {
private:
    int pc;
    std::string ir;
    Registers registers;
    ALU alu;
    bool halted;
    std::string screenOutput; 
public:
    CPU();

    // Getters & Setters
    int getPC() const;
    void setPC(int val);
    std::string getIR() const;
    bool isHalted() const;
    std::string getScreenOutput() const;
    std::string getRegister(int index) const;
    
    // GUI / Overwrite features
    void overwriteRegister(int index, const std::string& val);
    void reset();

    // Execution Core
    void fetch(Memory& mem);
    bool validate(const std::string& instruction) const;
    void execute(Memory& mem);
    
    // Control loops
    void step(Memory& mem);
    void run(Memory& mem);

    void displayStatus() const;
};

#endif
