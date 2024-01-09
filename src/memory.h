#include <iostream>
#include <map>
#include <string>

#define MAX_LINE_LENGTH 16
#define MAX_CODE_LINES 10
#define MAX_CODE_SIZE MAX_CODE_LINES * MAX_LINE_LENGTH
#define MAX_DATA_SIZE 0xFF + 1

#define MEMORY_COST_STORED_LABEL 1
#define MEMORY_COST_STORED_INSTRUCTIONS 1
#define MEMORY_COST_CACHE_HIT 1
#define MEMORY_COST_CACHE_MISS 2

class Memory {
private:
    uint32_t *codeBaseAddr;
    uint8_t *dataBaseAddr;
    //TODO: Type can be reduced to uint8_t
    std::map<uint32_t, int> jumpOffsets;

    bool isValidCodeAddr(int offset);
    bool isValidDataAddr(int offset);

public:
    int storedCode;
    int memoryCost;

    Memory();
    ~Memory();

    uint32_t codeRead(int offset);
    void codeWrite(int offset, uint32_t data);

    uint8_t dataRead(int offset);
    void dataWrite(int offset, uint8_t data);

    void addJumpOffset(uint32_t label, int offset);
    int findJumpOffset(uint32_t label);
    void printJumpOffsets(); //For Debugging 
};