#ifndef REGISTERS_H
#define REGISTERS_H

#include <iostream>
#include <cstdint>

class Registers {
private:
    uint8_t reg[16]; 
public:
    Registers(); 
    uint8_t getValue(int i);         
    void setValue(int i, uint8_t val); 
    void display();            

#endif