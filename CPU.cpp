#include "CPU.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

static int hexToInt(const std::string& hexStr) {
    unsigned int x = 0;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> x;
    return static_cast<int>(x);
}

static std::string intToHex(int val, int width = 2) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::uppercase << std::hex << (val & 0xFF);
    return ss.str();
}

CPU::CPU() : pc(0), ir("0000"), halted(false), screenOutput("") {}

int CPU::getPC() const { return pc; }
void CPU::setPC(int val) { pc = val; }
std::string CPU::getIR() const { return ir; }
bool CPU::isHalted() const { return halted; }
std::string CPU::getScreenOutput() const { return screenOutput; }

std::string CPU::getRegister(int index) const {
    return registers.getRegister(index);
}

void CPU::overwriteRegister(int index, const std::string& val) {
    registers.setRegister(index, val);
}

void CPU::reset() {
    pc = 0;
    ir = "0000";
    halted = false;
    screenOutput = "";
    for (int i = 0; i < 16; ++i) {
        registers.setRegister(i, "00");
    }
}

void CPU::fetch(Memory& mem) {
    if (halted) return;
    
    if (pc > 254) {
        halted = true;
        return;
    }

    uint8_t b1 = mem.read(static_cast<uint8_t>(pc));
    uint8_t b2 = mem.read(static_cast<uint8_t>(pc + 1));

    ir = intToHex(b1, 2) + intToHex(b2, 2);
    pc += 2;
}

bool CPU::validate(const std::string& instruction) const {
    if (instruction.length() != 4) return false;
    for (char c : instruction) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
    }

    unsigned int val = 0;
    std::stringstream ss;
    ss << std::hex << instruction;
    ss >> val;

    return Instructions::validate(static_cast<uint16_t>(val));
}

void CPU::execute(Memory& mem) {
    if (halted) return;

    if (!validate(ir)) {
        std::cout << "[ERROR] Invalid instruction in IR: " << ir << std::endl;
        halted = true;
        return;
    }

    char op = ir[0];
    int r = hexToInt(std::string(1, ir[1]));
    int x = hexToInt(std::string(1, ir[2]));
    int y = hexToInt(std::string(1, ir[3]));
    int xy = hexToInt(ir.substr(2, 2));

    switch (op) {
        case '1': {
            uint8_t memVal = mem.read(static_cast<uint8_t>(xy));
            registers.setRegister(r, static_cast<int>(memVal));
            break;
        }
        case '2': { 
            registers.setRegister(r, ir.substr(2, 2));
            break;
        }
        case '3': { 
            uint8_t valToStore = hexToInt(registers.getRegister(r));
            mem.write(static_cast<uint8_t>(xy), valToStore);
            if (xy == 0xFF) {
                std::string outVal = registers.getRegister(r);
                screenOutput += outVal + "\n";
                std::cout << "\n[SCREEN OUTPUT]: " << outVal << "\n";
            }
            break;
        }
        case '4': { 
            registers.setRegister(y, registers.getRegister(x));
            break;
        }
        case '5': {
            int val1 = hexToInt(registers.getRegister(x));
            int val2 = hexToInt(registers.getRegister(y));
            int res = alu.addInteger(val1, val2);
            registers.setRegister(r, res);
            break;
        }
        case '6': { 
            int val1 = hexToInt(registers.getRegister(x));
            int val2 = hexToInt(registers.getRegister(y));
            int res = alu.addFloat(val1, val2);
            registers.setRegister(r, res);
            break;
        }
        case '7': { 
            int val1 = hexToInt(registers.getRegister(x));
            int val2 = hexToInt(registers.getRegister(y));
            int res = alu.bitwiseOr(val1, val2);
            registers.setRegister(r, res);
            break;
        }
        case '8': { 
            int val1 = hexToInt(registers.getRegister(x));
            int val2 = hexToInt(registers.getRegister(y));
            int res = alu.bitwiseAnd(val1, val2);
            registers.setRegister(r, res);
            break;
        }
        case '9': { 
            int val1 = hexToInt(registers.getRegister(x));
            int val2 = hexToInt(registers.getRegister(y));
            int res = alu.bitwiseXor(val1, val2);
            registers.setRegister(r, res);
            break;
        }
        case 'A': { // Rotate register R right Y times (using ALU)
            int val = hexToInt(registers.getRegister(r));
            int res = alu.rotateRight(val, y); 
            registers.setRegister(r, res);
            break;
        }
        case 'B': { 
            int rVal = hexToInt(registers.getRegister(r));
            int r0Val = hexToInt(registers.getRegister(0));
            if (rVal == r0Val) {
                pc = xy;
            }
            break;
        }
        case 'C': { 
            halted = true;
            break;
        }
        default:
            halted = true;
            break;
    }
}

void CPU::step(Memory& mem) {
    if (halted) return;
    fetch(mem);
    if (!halted) {
        execute(mem);
    }
}

void CPU::run(Memory& mem) {
    while (!halted) {
        step(mem);
    }
}

void CPU::displayStatus() const {
    std::cout << "\nPC: 0x" << intToHex(pc) << " | IR: " << ir << " | Halted: " << (halted ? "True" : "False") << std::endl;
    registers.displayRegisters();
}
