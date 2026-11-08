#include "CPU.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

static int hexToInt(const std::string& hexStr) {
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> x;
    return x;
}

static std::string intToHex(int val, int width = 2) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::uppercase << std::hex << val;
    return ss.str();
}

CPU::CPU() : pc(0), ir("0000"), halted(false) {}

int CPU::getPC() const { return pc; }
void CPU::setPC(int val) { pc = val; }
std::string CPU::getIR() const { return ir; }
bool CPU::isHalted() const { return halted; }

void CPU::fetch(Memory& mem) {
    if (pc >= 254) {
        halted = true;
        return;
    }
    ir = mem.getCell(pc) + mem.getCell(pc + 1);
    pc += 2;
}

bool CPU::validate(const std::string& inst) const {
    if (inst.length() != 4) return false;
    for (char c : inst) {
        if (!isxdigit(c)) return false;
    }
    char op = inst[0];
    if ((op >= '1' && op <= '9') || op == 'A' || op == 'B' || op == 'C') {
        return true;
    }
    return false;
}

void CPU::execute(Memory& mem) {
    if (!validate(ir)) {
        std::cout << "[ERROR] Invalid instruction: " << ir << std::endl;
        halted = true;
        return;
    }

    char op = ir[0];
    int r = hexToInt(std::string(1, ir[1]));
    int x = hexToInt(std::string(1, ir[2]));
    int y = hexToInt(std::string(1, ir[3]));
    int xy = hexToInt(ir.substr(2, 2));

    switch (op) {
        case '1':
            registers.setRegister(r, mem.getCell(xy));
            break;
        case '2':
            registers.setRegister(r, ir.substr(2, 2));
            break;
        case '3':
            mem.setCell(xy, registers.getRegister(r));
            if (xy == 0xFF) {
                std::cout << "\n[SCREEN OUTPUT]: " << registers.getRegister(r) << "\n";
            }
            break;
        case '4':
            registers.setRegister(y, registers.getRegister(x));
            break;
        case '5': {
            int val1 = hexToInt(registers.getRegister(x));
            int val2 = hexToInt(registers.getRegister(y));
            int res = (val1 + val2) % 256;
            registers.setRegister(r, intToHex(res));
            break;
        }
        case 'B':
            if (registers.getRegister(r) == registers.getRegister(0)) {
                pc = xy;
            }
            break;
        case 'C':
            halted = true;
            break;
        default:
            halted = true;
            break;
    }
}

void CPU::displayStatus() const {
    std::cout << "\nPC: " << intToHex(pc) << " | IR: " << ir << std::endl;
    registers.displayRegisters();
}