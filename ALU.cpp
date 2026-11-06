#include "ALU.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

uint8_t ALU::addinteger(uint8_t a , uint8_t b) {
    result = a+b;
    return result;
}
uint8_t ALU::addfloating(uint8_t a , uint8_t b) {

    int signA = (a>>7) & 1;
    int expA = ((a>>4) & 7)-4;
    int mantA = a & 15;
    int signB = (b>>7) & 1;
    int expB = ((b>>4) & 7)-4;
    int mantB = b & 15;

    double valuA = (mantA/16.0) * pow(2,expA);
    double valuB = (mantB/16.0) * pow(2,expB);

    if (signA == 1) {
        valuA = -valuA;
    }
    if (signB == 1) {
        valuB = -valuB;
    }

    double Result = valuA + valuB;

    if (Result==0) {
        result =0;
        return result;
    }

    int sign=0;
    if (Result<0) {
        sign=1;
        Result=-Result;
    }

    int exponent=-4;
    int mantissa;
    mantissa = round(Result * pow(2,4-exponent));

    while (mantissa>15 && exponent < 3) {
        exponent++;
        mantissa = round(Result * pow(2,4-exponent));
    }
    while (mantissa<8 && exponent > -4) {
        exponent--;
        mantissa = round(Result * pow(2,4-exponent));
    }
    if (mantissa<8 || mantissa>15) {
        result=0;
        return result;
    }
    result=(sign<<7) | ((exponent+4)<<4) | mantissa;
    return result;

}
uint8_t ALU::bitwiseand(uint8_t a , uint8_t b) {
    result = a & b;
    return result;
}
uint8_t ALU::bitwiseor(uint8_t a , uint8_t b) {
    result = a | b;
    return result;
}
uint8_t ALU::bitwisexor(uint8_t a , uint8_t b) {
    result = a ^ b;
    return result;
}
uint8_t ALU::rotateright(uint8_t value, uint8_t steps) {
    steps%=8;
    result = (value >> steps) | (value << (8-steps));
    return result;
}
