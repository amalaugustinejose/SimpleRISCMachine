#include <string>
#include <iostream>
#include "cpu.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "Please provide input filename" << endl;
        return 0;
    }
    
    static Cpu cpu;

    string fileName(argv[1]);
    cpu.storeInputProgram(fileName);
    //cpu.readStoredProgram();
    //cpu.startExecution();
    uint64_t regDump = cpu.executeAndDump();
    std::cout << "Reg Dump: 0x" << std::hex << regDump << endl;

    return 0;
}