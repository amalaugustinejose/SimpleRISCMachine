#include <iostream>
#include "memory.h"

//#define CACHE_ENABLED

Memory::Memory() {
    codeBaseAddr = new uint32_t [MAX_CODE_SIZE];
    dataBaseAddr = new uint8_t [MAX_DATA_SIZE];
    storedCode = 0; // Bound for used code segment, also to facilitate HALT
    lru = new LRUCache(LRU_CACHE_SIZE);
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
        memoryCost += MEMORY_COST_STORED_INSTRUCTIONS;
        *(codeBaseAddr + offset) = data;
        storedCode++;
        //std::cout << memoryCost << ' ' << storedCode << std::endl;
    } else
        std::cout << "Memory::codeWrite Invalid offset" << std::endl;
}

#ifdef CACHE_ENABLED
uint8_t Memory::dataRead(int offset) {
    if (isValidDataAddr(offset)) {
        memoryCost += MEMORY_COST_CACHE_HIT;
        std::cout << "Memory::dataRead @" << offset << ' ' << (int) *(dataBaseAddr + offset) << std::endl;
        return *(dataBaseAddr + offset);
    } else
        std::cout << "Memory::dataRead Invalid offset" << std::endl;
    return 0x0;
}
#else
uint8_t Memory::dataRead(int offset) {
    LRUInfo response = {0};
    if (isValidDataAddr(offset)) {
        //uint8_t data = lru.get(offset);
        response = this->lru->get(offset);
        if (response.isCacheMiss) { // Cache MISS
            std::cout << "Memory::dataRead Cache MISS" << std::endl;
            memoryCost += MEMORY_COST_CACHE_MISS;
            response = this->lru->put(offset, *(dataBaseAddr + offset));
            if (response.cacheReplaced) {
                std::cout << "Memory::dataRead Cache Replaced" << std::endl;
                if (isValidDataAddr(response.offset)) {
                    *(dataBaseAddr + response.offset) = response.data;
                    std::cout << "Memory::dataRead Cache Write - Back @" << response.offset
                        << ' ' << (int) response.data << std::endl; 
                }
            }
            std::cout << "Memory::dataRead @" << offset
                    << ' ' << (int) *(dataBaseAddr + offset) << std::endl;
            return *(dataBaseAddr + offset);            
        } else { // Cache HIT
            std::cout << "Memory::dataRead Cache HIT" << std::endl;
            memoryCost += MEMORY_COST_CACHE_HIT;
            std::cout << "Memory::dataRead @" << response.offset
                    << ' ' << (int) response.data << std::endl;
            return response.data;
        }
    } else
        std::cout << "Memory::dataRead Invalid offset" << std::endl;
    return 0x0;
}
#endif

#ifdef CACHE_ENABLED
void Memory::dataWrite(int offset, uint8_t data) {
    if (isValidDataAddr(offset)) {
        memoryCost += MEMORY_COST_CACHE_HIT;
        *(dataBaseAddr + offset) = data;
        std::cout << "Memory::dataWrite @" << offset << ' ' << (int) data << ' ' << (int) *(dataBaseAddr + offset) << std::endl;
    } else
        std::cout << "Memory::dataWrite Invalid offset" << std::endl;
}
#else
void Memory::dataWrite(int offset, uint8_t data) {
    if (isValidDataAddr(offset)) {
        LRUInfo response = {0};
        response = lru->put(offset, data);
        if (response.isCacheMiss) {
            memoryCost += MEMORY_COST_CACHE_MISS;
            std::cout << "Memory::dataWrite Cache MISS" << std::endl;
        } else {
            memoryCost += MEMORY_COST_CACHE_HIT;
            std::cout << "Memory::dataWrite Cache HIT" << std::endl;
        }
        if (response.cacheReplaced) {
            std::cout << "Memory::dataWrite Cache Replaced" << std::endl;
                if (isValidDataAddr(response.offset)) {
                    *(dataBaseAddr + response.offset) = response.data;
                    std::cout << "Memory::dataWrite Cache Write - Back @" << response.offset
                        << ' ' << (int) response.data << std::endl; 
                }
            }
        // *(dataBaseAddr + offset) = data; // Since using Write-Back not updated to storage
        std::cout << "Memory::dataWrite @" << offset << ' ' << (int) data << ' ' << (int) *(dataBaseAddr + offset) << std::endl;
    } else
        std::cout << "Memory::dataWrite Invalid offset" << std::endl;
}
#endif

void Memory::addJumpOffset(uint32_t label, int offset) {
    //std::cout << "Memory::addJumpOffset" << (int) label << std:: endl;
    memoryCost += MEMORY_COST_STORED_LABEL;
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