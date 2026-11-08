#ifndef CPU_H
#define CPU_H

#include <string>
#include <iostream>
#include "Memory.h"
#include "Registers.h"

class CPU {
private:
    int pc;
    std::string ir;
    Registers registers;
    bool halted;

public:
    CPU();

    int getPC() const;
    void setPC(int val);
    std::string getIR() const;
    bool isHalted() const;

    void fetch(Memory& mem);
    bool validate(const std::string& instruction) const;
    void execute(Memory& mem);

    void displayStatus() const;
};

#endif