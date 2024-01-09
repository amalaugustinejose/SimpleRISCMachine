// | - 1Byte Operation - | -  12 bits Operand 1 - | - 12 bits Operand 2 - |               
//                    |                             |
//                 |                                   | 
//              | - 4 bits OpType - | - 2 Bytes OpValue - |

#define OP_SHIFT 24 // 6 Bytes

#define OP1_MASK 0x00fff000
#define OP1_SHIFT 12 
 
#define OP2_MASK 0x00000fff
#define OP2_SHIFT 0

#define OPR_TYPE_SHIFT 8
#define OPR_VAL_MASK 0x0ff

enum instructions { // 1 Byte
    MOV = 0x01,
    ADD = 0x02,
    SUB = 0X03,
    CMP = 0X04,
    JMP = 0x05,
    LOD = 0x06,
    STR = 0x07,
    MUL = 0x08,
    DIV = 0x09,
    LSH = 0x0A,
    RSH = 0x0B,
    AND = 0x0C,
    OR  = 0x0D,
    XOR = 0x0E,
    NOT = 0x0F,
    HLT = 0x10,
    NOP = 0x11,
    LBL = 0xFF
};

enum operandType { // 4 bits
    NO_OP   = 0x0,
    REG     = 0x1,
    ADR     = 0x2,
    DAT     = 0x3,
    INVALID = 0xF
};

enum jumpType { // 4 bits (Operand Type in JUMP case)
    NO_CON  = 0x0,
    N_ZERO  = 0x1
};

enum dataRegTag {
    R0  = 0x0,
    R1  = 0x1,
    R2  = 0x2,
    R3  = 0x3
};

//https://developer.arm.com/documentation/ddi0601/2022-09/AArch64-Registers/NZCV--Condition-Flags
enum statusRegTag {
    N = 0x0,
    Z = 0x1,
    C = 0x2,
    V = 0x3 // Used denote Divide by Zero and if Operand 2 is greater
};