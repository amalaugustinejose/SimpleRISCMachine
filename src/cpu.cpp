#include <iostream>
#include "cpu.h"
#include "instr_set.h"

struct Cpu::decodedOps {
    uint8_t op1Type, op2Type;
    uint16_t op1Val, op2Val;
};

void Cpu::printRegs() {
    std::cout << std::endl << "Data Registers" << std::endl;
    std::cout << "R0: " << (int) dataRegs[R0] << std::endl;
    std::cout << "R1: " << (int) dataRegs[R1] << std::endl;
    std::cout << "R2: " << (int) dataRegs[R2] << std::endl;
    std::cout << "R3: " << (int) dataRegs[R3] << std::endl;

    std::cout << "Status Registers" << std::endl;
    std::cout << "N: " << (int) statusRegs[N] << std::endl;
    std::cout << "Z: " << (int) statusRegs[Z] << std::endl;
    std::cout << "C: " << (int) statusRegs[C] << std::endl;
    std::cout << "V: " << (int) statusRegs[V] << std::endl;
    std::cout << std::endl;
}

struct Cpu::decodedOps Cpu::decodeOprs(uint32_t op) {
    //std::cout << (int) op << std::endl;
    //std::cout  << (int)((op & OP1_MASK))<< std::endl;;
    struct Cpu::decodedOps ops;
    uint32_t op1 = (op & OP1_MASK) >> OP1_SHIFT;
    uint32_t op2 = (op & OP2_MASK) >> OP2_SHIFT;

    //std::cout << std::hex << op1 << std::endl;
    //std::cout << std::hex << op2 << std::endl;

    uint8_t op1Type, op2Type;
    //std::cout << (int) op1 << ' ' << (op1 >> OPR_TYPE_SHIFT) << std::endl;
    if (((op1 >> OPR_TYPE_SHIFT) & REG) == REG) {
        ops.op1Type = REG;
        //std::cout << "OP1 Type: REG" << std::endl; 
    } else if (((op1 >> OPR_TYPE_SHIFT) & ADR) == ADR) {
        ops.op1Type = ADR;
        //std::cout << "OP1 Type: ADR" << std::endl; 
    } else if (((op1 >> OPR_TYPE_SHIFT) & DAT) == DAT) {
        ops.op1Type = DAT;
        //std::cout << "OP1 Type: DAT" << std::endl; 
    } else if (((op1 >> OPR_TYPE_SHIFT) & NO_OP) == NO_OP) {
        ops.op1Type = NO_OP;
        //std::cout << "OP1 Type: No Operand" << std::endl; 
    } else {
        ops.op1Type = INVALID;
        //std::cout << "OP1 Type: INVALID" << std::endl;
        //TODO: Maybe set to flag if REQUIRED 
    }

    //std::cout << (int) op2 << ' ' << (op2 >> OPR_TYPE_SHIFT) << std::endl;
    if (((op2 >> OPR_TYPE_SHIFT)) == REG) {
        ops.op2Type = REG;
        //std::cout << "OP2 Type: REG" << std::endl; 
    } else if (((op2 >> OPR_TYPE_SHIFT)) == ADR) {
        ops.op2Type = ADR;
        //std::cout << "OP2 Type: ADR" << std::endl; 
    } else if (((op2 >> OPR_TYPE_SHIFT)) == DAT) {
        ops.op2Type = DAT;
        //std::cout << "OP2 Type: DAT" << std::endl; 
    } else if (((op2 >> OPR_TYPE_SHIFT) ) == NO_OP) {
        ops.op2Type = NO_OP;
        //std::cout << "OP2 Type: No Operand" << std::endl; 
    } else {
        ops.op2Type = INVALID;
        //std::cout << "OP2 Type: INVALID" << std::endl;
        //TODO: Maybe set to flag if REQUIRED 
    }
    //std::cout << "OP1 Type: " << (int) ops.op1Type << std::endl;
    //std::cout << "OP2 Type: " << (int) ops.op2Type << std::endl;

    ops.op1Val = (op1 & OPR_VAL_MASK);
    ops.op2Val = (op2 & OPR_VAL_MASK);

    //std::cout << "OP1 Val: " << ops.op1Val << std::endl;
    //std::cout << "OP2 Val: " << ops.op2Val << std::endl;

    return ops;
}

void Cpu::executeOp(uint32_t op) {
    //std::cout << "CPU: InPut" << (int) op << std::endl;

    //Decode Operation
    if (((op >> OP_SHIFT)) == MOV) {
        std::cout << "CPU: MOV" << std::endl;
        mov(op);
    } else if (((op >> OP_SHIFT)) == ADD) {
        std::cout << "CPU: ADD" << std::endl;
        add(op);
    } else if (((op >> OP_SHIFT)) == SUB) {
        std::cout << "CPU: SUB" << std::endl;
        sub(op);
    } else if (((op >> OP_SHIFT)) == CMP) {
        std::cout << "CPU: CMP" << std::endl;
        cmp(op);
    } else if (((op >> OP_SHIFT)) == JMP) {
        std::cout << "CPU: JMP" << std::endl;
        jmp(op);
    } else if (((op >> OP_SHIFT)) == LOD) {
        std::cout << "CPU: LOD" << std::endl;
        load(op);
    } else if (((op >> OP_SHIFT)) == STR) {
        std::cout << "CPU: STR" << std::endl;
        store(op);
    } else if (((op >> OP_SHIFT)) == MUL) {
        std::cout << "CPU: MUL" << std::endl;
        mult(op);
    } else if (((op >> OP_SHIFT)) == DIV) {
        std::cout << "CPU: DIV" << std::endl;
        div(op);
    } else if (((op >> OP_SHIFT)) == LSH) {
        std::cout << "CPU: LSH" << std::endl;
        lshift(op);
    } else if (((op >> OP_SHIFT)) == RSH) {
        std::cout << "CPU: RSH" << std::endl;
        rshift(op);
    } else if (((op >> OP_SHIFT)) == AND) {
        std::cout << "CPU: AND" << std::endl;
        land(op);
    } else if (((op >> OP_SHIFT)) == OR) {
        std::cout << "CPU: OR" << std::endl;
        lor(op);
    } else if (((op >> OP_SHIFT)) == XOR) {
        std::cout << "CPU: XOR" << std::endl;
        lxor(op);
    } else if (((op >> OP_SHIFT)) == NOT) {
        std::cout << "CPU: NOT" << std::endl;
        lnot(op);
    } else if (((op >> OP_SHIFT)) == HLT) {
        std::cout << "CPU: HLT" << std::endl;
        programCounter = mem.storedCode; // This will terminate execution
    } else if (((op >> OP_SHIFT)) == NOP) {
        std::cout << "CPU: NOP" << std::endl;
    } else {
        std::cout << "CPU: Invalid: " << (op >> OP_SHIFT) << ' ' << NOP << std::endl;
    }
}

void Cpu::mov(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Move Logic
    if (ops.op1Type == REG && ops.op2Type == DAT) {
        if (ops.op1Val < CPU_DATA_REGISTER_COUNT) {
            dataRegs[ops.op1Val] = ops.op2Val;
        }
    } else if (ops.op1Type == REG && ops.op2Type == REG) {
        if (ops.op1Val < CPU_DATA_REGISTER_COUNT && ops.op1Val < CPU_DATA_REGISTER_COUNT) {
            dataRegs[ops.op1Val] = dataRegs[ops.op2Val];
        }
    }
    //this->printRegs();
    //TODO: Ops involving memory
}

void Cpu::add(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Add logic
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] + dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] + ops.op2Val;
    }
    std::cout << "Sum: " << (int) dataRegs[ops.op1Val] << std::endl;
    //this->printRegs();
    //TODO: Ops involving memory
    //TODO: Overflow and carry cases
}

void Cpu::sub(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Subtract logic
    //TODO: Add a function to reset status
    statusRegs[Z] = 0; // Result is Zero
    statusRegs[N] = 0; // Result is less than Zero Negative // Not supported
    if (dataRegs[ops.op1Val] > dataRegs[ops.op2Val]) // Difference will be negative
        statusRegs[N] = 0;
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] - dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] - ops.op2Val;
    } else {
        return; // To make sure it does not accidnelty set Z status
    }
    std::cout << "Difference: " << (int) dataRegs[ops.op1Val] << std::endl;
    if (dataRegs[ops.op1Val] == 0)
        statusRegs[Z] = 1;
    // this->printRegs();
    //TODO: Ops involving memory
    //TODO: Negative result cases    
}

void Cpu::mult(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Multiply Logic
    statusRegs[C] = 0; // Unsigned overflow
    uint16_t product = 0x0;
    if (ops.op1Type == REG && ops.op2Type == REG) {
        product = dataRegs[ops.op1Val] * dataRegs[ops.op2Val];
        dataRegs[ops.op1Val] = product;
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        product = dataRegs[ops.op1Val] * ops.op2Val;
        dataRegs[ops.op1Val] = product;
    } else {
        return; // To make sure it does not accidnelty set Z status
    }
    if (product > 0xff)
        statusRegs[C] = 1;
    std::cout << "Product: " << product << " DR " << (int) dataRegs[ops.op1Val] << std::endl;
    //this->printRegs();
}

void Cpu::div(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Multiply Logic
    statusRegs[Z] = 0; // Check is quotient is Zero
    statusRegs[V] = 0; // Divide By Zero status clear
    if (ops.op2Val == 0) {
        std::cout << "Quotient: DIVIDE BY ZERO" << std::endl;
        statusRegs[V] = 0x1;
        return;
    }

    //this->printRegs();
    uint16_t quotient = 0xFF;
    if (ops.op1Type == REG && ops.op2Type == REG) {
        quotient = dataRegs[ops.op1Val] / dataRegs[ops.op2Val];
        dataRegs[ops.op1Val] = quotient;
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        quotient = dataRegs[ops.op1Val] / ops.op2Val;
        dataRegs[ops.op1Val] = quotient;
    } else {
        return; // To make sure it does not accidnelty set Z status
    }
    if (quotient == 0)
        statusRegs[Z] = 0x1;
    std::cout << "Quotient: " << quotient << " DR: " << (int) dataRegs[ops.op1Val] << std::endl;
    //this->printRegs();
}

void Cpu::lshift(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //this->printRegs();
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] << dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] << ops.op2Val;
    } else {
        return;
    }
    //this->printRegs();
}

void Cpu::rshift(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //this->printRegs();
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] >> dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] >> ops.op2Val;
    } else {
        return;
    }
    //this->printRegs();
}

void Cpu::jmp(uint32_t op) {
    // Operand 1 JMP Type and Operand 2 JMP Label
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Do jump logic
    //std::cout << "Cpu::jmp " << (int) op << ' ' << (int) ops.op1Type << std::endl;
    // std::cout << "Cpu::jmp Label" << (std::hex) << ops.op2Val << std::endl;
    if (ops.op1Type == NO_CON) { // && ops.op2Type == DAT) { OP2 is not considered for jump
            std::cout << "Cpu::jmp NO_CON INVOKED" << std::endl;
            programCounter = mem.findJumpOffset(ops.op2Val) - 1;
            //Jump to ops.op1Val
            //This will need a redeisgn since CPU does not have access to jump offsets which are part of stored class 
    } else if (ops.op1Type == N_ZERO) {
        //std::cout << "Cpu::jmp ZERO INVOKED 2" << std::endl;
        if (statusRegs[Z] == 0) {
            programCounter = mem.findJumpOffset(ops.op2Val) - 1;
        }
    }
    // this->printRegs();
}

void Cpu::cmp(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //Do the comp logic
    statusRegs[N] = 0x0; // Will be set if OP1 is less
    statusRegs[Z] = 0x0; // Will be set if both equal
    statusRegs[V] = 0x0; // Will be set if OP1 is greater
    if (ops.op1Type == REG && ops.op2Type == REG) {
        if (dataRegs[ops.op1Val] > dataRegs[ops.op2Val]) {
            std::cout << "CMP: OP1 > OP2" << std::endl;
            statusRegs[V] = 0x1;
        } else if (dataRegs[ops.op1Val] < dataRegs[ops.op2Val]) {
            std::cout << "CMP: OP1 < OP2" << std::endl;
            statusRegs[N] = 0x1;
        } else {
            std::cout << "CMP: OP1 == OP2" << std::endl;
            statusRegs[Z] = 0x1;
        }
    }
}

void Cpu::load(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Move Logic
    if (ops.op1Type == REG && ops.op2Type == ADR) {
        if (ops.op1Val < CPU_DATA_REGISTER_COUNT) {
            //std::cout << "Cpu:Load: Load to Register" << std::endl;
            dataRegs[ops.op1Val] = mem.dataRead(ops.op2Val);
        }
    } else {

        std::cout << "Cpu:Load: Invalid Load OP" << std::endl;
    }
}

void Cpu::store(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    // Move Logic
    if (ops.op1Type == REG && ops.op2Type == ADR) {
        if (ops.op1Val < CPU_DATA_REGISTER_COUNT) {
            //std::cout << "Cpu:Load: Store from Register" << std::endl;
            mem.dataWrite(ops.op2Val, dataRegs[ops.op1Val]);
        }
    } else {
        std::cout << "Cpu:Store: Invalid Store OP" << std::endl;
    }
    //this->printRegs();
}

void Cpu::land(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //this->printRegs();
    statusRegs[Z] = 0;
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] & dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] & ops.op2Val;
    } else {
        std::cout << "Cpu::land: Invalid AND OP" << std::endl;
        return;
    }
    if (dataRegs[ops.op1Val] == 0)
        statusRegs[Z] = 0x1;
    //this->printRegs();
}

void Cpu::lor(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //this->printRegs();
    statusRegs[Z] = 0;
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] | dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] | ops.op2Val;
    } else {
        std::cout << "Cpu::land: Invalid OR OP" << std::endl;
        return;
    }
    if (dataRegs[ops.op1Val] == 0)
        statusRegs[Z] = 0x1;
    //this->printRegs();
}

void Cpu::lxor(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //this->printRegs();
    statusRegs[Z] = 0;
    if (ops.op1Type == REG && ops.op2Type == REG) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] ^ dataRegs[ops.op2Val];
    } else if (ops.op1Type == REG && ops.op2Type == DAT) {
        dataRegs[ops.op1Val] = dataRegs[ops.op1Val] ^ ops.op2Val;
    } else {
        std::cout << "Cpu::land: Invalid XOR OP" << std::endl;
        return;
    }
    if (dataRegs[ops.op1Val] == 0)
        statusRegs[Z] = 0x1;
    //this->printRegs();
}

void Cpu::lnot(uint32_t op) {
    struct Cpu::decodedOps ops = decodeOprs(op);
    //this->printRegs();
    statusRegs[Z] = 0;
    if (ops.op1Type == REG) {
        dataRegs[ops.op1Val] = ~dataRegs[ops.op1Val];
    } else {
        std::cout << "Cpu::land: Invalid NOT OP" << std::endl;
        return;
    }
    if (dataRegs[ops.op1Val] == 0)
        statusRegs[Z] = 0x1;
    //this->printRegs();
}

void Cpu::startExecution() {
    std::cout << std::endl << std::endl << "Cpu::startExecution BEGIN" << std::endl;
    //this->printRegs();

    this->programCounter = 0;
    while(watchDog-- && this->programCounter < this->mem.storedCode) {
        //std::cout << "PC: " << programCounter << std::endl;
        executeOp(this->mem.codeRead(this->programCounter++));
    }
    std::cout << std::endl << std::endl << "Cpu::startExecution END" << std::endl;
    //this->printRegs();
}

uint64_t Cpu::createRegDump () {
    uint32_t dataRegCombine = 0x0;
    uint32_t statusRegCombine = 0x0;
    uint64_t dataStatusCombine = 0x0;

    dataRegCombine |= dataRegs[R0];
    dataRegCombine <<= 8;
    dataRegCombine |= dataRegs[R1];
    dataRegCombine <<= 8;
    dataRegCombine |= dataRegs[R2];
    dataRegCombine <<= 8;
    dataRegCombine |= dataRegs[R3];

    statusRegCombine |= statusRegs[N];
    statusRegCombine <<= 8;
    statusRegCombine |= statusRegs[Z];
    statusRegCombine <<= 8;
    statusRegCombine |= statusRegs[C];
    statusRegCombine <<= 8;
    statusRegCombine |= statusRegs[V];

    dataStatusCombine |= dataRegCombine;
    dataStatusCombine <<= 32;
    dataStatusCombine |= statusRegCombine;

    std::cout << std::hex << dataStatusCombine << std::endl;
    return dataStatusCombine;
}

uint64_t Cpu::executeAndDump() {
    startExecution();
    return createRegDump();
}

uint64_t Cpu::executeOpDump(uint32_t op) {
    executeOp(op);
    return createRegDump();
}

void Cpu::storeInputProgram(std::string filename) {
    std::fstream ipFile;
    ipFile.open(filename);
    if (ipFile.fail()) {
        std::cout << "Failed to open " << filename << std::endl;
        return;
    }
    std::string line;
    uint32_t uLine;
    int offset = 0;
    while(getline(ipFile, line)) {
        if (line.length() == 0 || line[0] == '#')
            continue;
        std::stringstream ss;
        ss << std::hex << line;
        ss >> uLine;
        // 255 Label at Max
        if (line[0] == 'F') {
            // Extract Label (LBL) mas 0x000FF000
            // Labels limited from 0x00 - 0xFF (255 Nos)
            //TODO: The type can be reduced to uint8_t
            uint32_t labelId = (uLine & 0x00ff000) >> 12;
            mem.addJumpOffset(labelId, mem.storedCode + 1);
        } else {
            mem.codeWrite(offset, uLine);
            offset++;
        }
    }
    ipFile.close();
}

void Cpu::readStoredProgram() {
    std::cout << "Reading from Memory BLAH BLAH" << std::endl;
    //cout << cpu.mem.storedCode << endl;
    for (int i = 0; i < mem.storedCode; i++)
        std::cout << (mem.codeRead(i)) << std::endl;
    std::cout << "Printing jump offsets if any" << std::endl;
    mem.printJumpOffsets();
}