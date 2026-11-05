#ifndef REGISTERS_H
#define REGISTERS_H

#include <iostream>
#include <string>
#include <iomanip>

class Registers {
private:
    std::string registers[16];

public:
    Registers();
    void setRegister(int index, std::string val);
    void setRegister(int index, int val);
    std::string getRegister(int index) const;
    void displayRegisters() const;
};

#endif