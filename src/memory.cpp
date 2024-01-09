#include <iostream>
#include "memory.h"

Memory::Memory() {
    codeBaseAddr = new uint32_t [MAX_CODE_SIZE];
    dataBaseAddr = new uint8_t [MAX_DATA_SIZE];
    storedCode = 0; // Bound for used code segment, also to facilitate HALT
}

Memory::~Memory() {
    delete[] codeBaseAddr;
    delete[] dataBaseAddr;
}

bool Memory::isValidCodeAddr(int offset) {
    if (offset < MAX_CODE_SIZE)
        return true;
    else
        return false;
}

bool Memory::isValidDataAddr(int offset) {
    if (offset < MAX_DATA_SIZE)
        return true;
    else
        return false;
}

uint32_t Memory::codeRead(int offset) {
    if (isValidCodeAddr(offset))
        return *(codeBaseAddr + offset);
    else
        std::cout << "Memory::codeRead Invalid offset" << std::endl;
    return 0x0;
}

void Memory::codeWrite(int offset, uint32_t data) {
    if (isValidCodeAddr(offset)) {
        *(codeBaseAddr + offset) = data;
        storedCode++;
    } else
        std::cout << "Memory::codeWrite Invalid offset" << std::endl;
}

uint8_t Memory::dataRead(int offset) {
    if (isValidDataAddr(offset)) {
        std::cout << "Memory::dataRead @" << offset << ' ' << (int) *(dataBaseAddr + offset) << std::endl;
        return *(dataBaseAddr + offset);
    } else
        std::cout << "Memory::dataRead Invalid offset" << std::endl;
    return 0x0;
}

void Memory::dataWrite(int offset, uint8_t data) {
    if (isValidDataAddr(offset)) {
        *(dataBaseAddr + offset) = data;
        std::cout << "Memory::dataWrite @" << offset << ' ' << (int) data << ' ' << (int) *(dataBaseAddr + offset) << std::endl;
    } else
        std::cout << "Memory::dataWrite Invalid offset" << std::endl;
}

void Memory::addJumpOffset(uint32_t label, int offset) {
    //std::cout << "Memory::addJumpOffset" << (int) label << std:: endl;
    jumpOffsets[label] = offset;
}

int Memory::findJumpOffset(uint32_t label) {
    //std::cout << "Memory::findJumpOffset" << jumpOffsets[label];
    return jumpOffsets[label];
}

void Memory::printJumpOffsets() {
    for (auto it: jumpOffsets) {
        std::cout << (int) it.first << " : " << it.second << std::endl;;
    }
}