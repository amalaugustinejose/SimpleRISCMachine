#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "memory.h"

#define CPU_DATA_REGISTER_COUNT 4 // 2 bit addressing
#define CPU_STATUS_REGISTER_COUNT 4 // 2 bit addressing
class Cpu {
private:
    Memory mem;
    struct decodedOps;
    uint8_t dataRegs[CPU_DATA_REGISTER_COUNT];
    uint8_t statusRegs[CPU_STATUS_REGISTER_COUNT];
    int programCounter; // to facilitate JMPS
    int watchDog = MAX_CODE_LINES * 0xff; // to avoid infitnite loops
    
    void mov(uint32_t op);
    void add(uint32_t op);
    void sub(uint32_t op);
    void mult(uint32_t op);
    void div(uint32_t op);
    void lshift(uint32_t op);
    void rshift(uint32_t op);
    void jmp(uint32_t op);
    void cmp(uint32_t op);
    void load(uint32_t op);
    void store(uint32_t op);
    void land(uint32_t op);
    void lor(uint32_t op);
    void lxor(uint32_t op);
    void lnot(uint32_t op);

    void printRegs();
    uint64_t createRegDump();
    struct decodedOps decodeOprs(uint32_t op);
    void executeOp(uint32_t op); //MADE PUBLIC for GTEST;

public:
    uint64_t executeAndDump(); // To facilitate testing
    void startExecution();
    void storeInputProgram(std::string filename);
    void readStoredProgram();
    uint64_t executeOpDump(uint32_t op);   
};