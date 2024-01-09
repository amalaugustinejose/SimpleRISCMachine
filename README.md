# SimpleRISCMachine

The obejctive with this projest is to simulate a simple RISC machine in software with a few supported commands, create tests and test harness to test the system and to write simple programs what can be ran on the system here.

## Directory Structure

```
.
├── CMakeLists.txt
├── src
│   ├── cpu.cpp
│   ├── cpu.h
│   ├── instr_set.h
│   ├── main.cpp
│   ├── memory.cpp
│   ├── memory.h
│   └── memory_unit_test.cpp
├── test_gtest
│   └── test_system.cpp
└── test_prgms
    ├── test_cmp_val.txt
    ├── test_count_digits.txt
    ├── test_factorial.txt
    ├── test_fibonacci.txt
    ├── test_left_shift.txt
    ├── test_load_store.txt
    ├── test_log_and.txt
    ├── test_log_or.txt
    ├── test_log_xor_not.txt
    ├── test_right_shift.txt
    └── test_sum_sub.txt
```
The **CMakeLists.txt** contains instructions for CMake compiler.

The **src** direcotry contains all the soruce files, like the filename suggests,
cpu.* included the code related to the cpu, memory.* includes the code related to memory block and instr_set.h includes the instructions, operand type, data and status register details.

The **test_gtest** includes the system tests, for now it's nothign but single instruction tests for all the functional blocks except JMP, NOP, HLT and LBL.#

The **test_prgms** includes test programs with instructiosn achieving certain objectives. Might need to uncomment status register prints to see a individual operation specific prints.

## How to build

Two main prerequisites to build the project easily are cmake and gtest. The project is entirely wirtten in C++. CMake was used to handle the builds and gtest (Google Test) for the test harness. More details about gtest can be found here https://github.com/google/googletest 

    cmake .
    make
That's it for the building, this should generate two binaries, riscMachine and testsystem

## How to run / test

To try run the gtest simply execute the binary testSystem

    ./testSystem
This should run the tests (for all the operators with the exception of JMP, NOP, HLT and LBL. The why will be explained below)

To try run some programs (there are few pre made programs in /test_prgms) execute the riscMachine binary with the path to the program as first argument

    ./riscMachine test_prgms/test_fibonacci.txt

## Assumptions / Lmititations
1. The system only handles 1 Byte unsigned integers.
2. Operation resulting in a carry will raise the Carry Flag in the status registers
3. The system uses a 4 Byte instruction, 1st Byte for the operation and remaining 3 Bytes shared among Operand 1 and Operand 2
4. Instructions at most can have two operands 
5. Result of any operation, if any, will be sotred in Operand 1
6. For all instructions (except JMP, NOP, HLT), operand will be one of the 4 data registers
7. Operand 2 can be register or 1 Byte unsinged intereger data or address location (for LOD and STR operations)
8. Decision to use 1 Bytes for opcode was not based on a NEED but a choice made to prototype a large system in limited time. For it's curretn state 4 bits is more than enough
9. Decision to use 12 bits for each operand was again a design consideration for fast protoyping. For example we jsut need 2 bits to specify the type of operand, but 4 was used to easily encode with hex values and move on.
10. Decision to stick to a two operand instruction format was to resue a single decode functioon to work with all the instructions.
11. Auto test using gtest was not used for NOP, HLT, JMP, LBL operations, since these does not result in cahnges to data or status registers which could enable to verify it's functionality as in gtest (more details below)
12. Those functions can all be independelty tested using test programs (just have to uncomment the HLT and NOP in the test programs to see them in action)

## Test Suite and D/S Registers

As mnedtioned in the introduction, it makes use of gTest and compares against the values in data and status registers to see if an operation has resulted in the inteneded result. The memebr function `uint64_t executeOpDump(uint32_t op)` executes the 32 bit instruction and returns the values in data and status registers in the following format.

    | R0 | R1 | R2 | R3 | N | C | Z | V |

    | R0 | - Data Register #0   | N | - Status Register N (Negative) // Used here if Operand 1 < Operand 2,
    | R1 | - Data Register #0   | C | - Status Register C (Carry)
    | R2 | - Data Register #0   | Z | - Status Register Z (Zero)
    | R3 | - Data Register #0   | V | - Status Register V (Signed Overflow) // Used HERE for Divide By Zero

The design decision to limit the resister to 8 bits was to facilitate dumping all of it back and thus enable validating functionality. Is it ideal? No, but a choice made to ensure the task is complete within the specified timeframe.

## Instruction Format

As mentioned in the Assumptions / Limitations section, it uses 32 bit intrustion where the first 8 bits are dedicated for the opcode and the remainig 24 split among operand 1 and operand 2. The instruction at max can have two operands each having 12 bits each where the 1st 4 bit are to specify the operand type (Register, Data, Type or Address) and the remaining 8 bits to specify the operand. Due to the size data and address offsets if limited to 0xFF. The above is illustrated below, which should cleare things up a little bit more.

    |                 32 bit instruction                 |
    | 8 bit opcode | 12 bit operand 1 | 12 bit operand 2 |

    |           12 bit operand           |
    | 4 bit operand type | 8 bit operand |

    |           12 bit operand           |
    | 4 bit JMP type  | 8 bit JMP label  |

Since the system is designed with a goal to recive instruction encoded in binary, it has to be provided wiht instructions encoded in hex format. A better way to facilitate this would have been to write a translater, where in the end user can write commands as text and the translater, translates it to the hex code the machine can take in. But then again, in a time crunch anc be considered for improvements to the system at a later date. 

## Details about Implemented Instructions

For now it just includes the basic instructions and a few extra arithmetic and logical operations

| Symbol | Format               | Description                        | Comments      |
| ------ | -------------------- | ---------------------------------- | ----------- |
| MOV    | `MOV RX RY/DATA`     | Move to RX data in RY or DATA      |         |
| ADD    | `ADD RX RY/DATA`     | Add to RX data in RY or DATA       |         |
| SUB    | `SUB RX RY/DATA`     | Subtract from RX data in RY/DATA   |  |
| CMP    | `CMP RX RY`          | Compare Data in RX with RY         |  [Z] = 1 -> OP1 = OP2, [C] =  1 OP1 > OP2, [V] = 1 -> OP1 < OP2 |
| JMP    | `JMP jumptype label` | Jump to label based on jump type   |  jumptype can be N_CON (Unconditional) and N_ZERO (Non Zero). This can be extended|
| LOD    | `LOD RX offset`      | Load RX with data in memory location @offset   | Load byte of data |
| STR    | `STR RX offset`      | Store data in RX with data in location @offset | Store byte of data |
| MUL    | `MUL RX RY/DATA`     | Multiply with RX data in RY or DATA | Product Overflow sets [C] = 1|
| DIV    | `DIV RX RY/DATA`     | Divide RX with data in RY or DATA   | Divide by Zero sets [V] = 1|
| LSH    | `LSH RX RY/DATA`     | Left Shift RX by data in RY or DATA | |
| RSH    | `RSH RX RY/DATA`     | Right Shift RX by data in RY or DATA| |
| AND    | `AND RX RY/DATA`     | AND RX with data in RY or DATA      | |
| OR     | `OR RX RY/DATA`      | OR RX with data in RY or DATA       | |
| XOR    | `XOR RX RY/DATA`     | XOR RX with data in RY or DATA      | |
| NOT    | `NOT RX`             | Flip the bits in RX                 | |
| HLT    | `HLT`                | Stope Execution and exits           | |
| NOP    | `NOP`                | No Operation                        | |
| LBL    | `LBL offset`         | To specify labeles                  | |

## How it all works

Most of the functions are with self explanatory fuction names. But the gist of it comes down to two main methods or interaction with the system.

1. Provide and entire program and execute it.
    
    a. Provide a text file with a list of instructions in hex format and feed it as argument to riscMachine.

    b. `void storeInputProgram(std::string filename)` will store the fed program in the code segment discarding comments, empty lines and labels. Lables are stored seperately by `void addJumpOffset(uint32_t label, int offset)` for easier lookup with `int findJumpOffset(uint32_t label)`. The label is provided as an second operand in JMP instruction.

    c. Calling `void startExecution()` will start executiion and continues till the end of stored code is met. There is counter named `watchDog` which decrements with each instruction. This is maintained and cheked to exit out in case of infinite loops.

    d. Jumps are facilitated by overriding the `programCounter` which will point it to the code location right after the label provided by `int findJumpOffset(uint32_t label)`.

    e. When the system meets a halt operation it just sets the `programCounter` to the end of stored code maintained by `storedCode`. This would force `void startExecution()` to exit out of the loop and terminate the process.

    f. Pretty simple design to facilitate the objective. Can this be optimised? A lot given time and resources. Starting from the instruction structure and how jumps are handles int the system to begin with.

2. To provide single instruction and see if the status registers are set correclty or not.
    
    a. This was not in the original design plan, but was introduced to facilitate the current auto test mechanism. The machine obviously can't SEE console ouputs can read and compare againts. This lead to the development of `uint64_t executeOpDump(uint32_t op)`.

    b. As the name suggests, this function would execute a single operation and dump the data and status registers to evaluate the output. Pretty convinient way to execute operations one by one and check the status od the system afterwards. 

    c. This has been the basis of the test suite design and inputs in `test_system.cpp` are just 32 bit hex encoded instructions and 64 bit hex encoded expected outputs. 

## Pending Improvements and Upgrades

1. Translator to take in instructions weitten in human languge and translate it to binary encoded instructions.
2. Remove limitations of two operands, there are enough bits in the 32 bit instruction to pack in another operand. This could facilitate to specify where to store the result (if any), rather than to store it in the operand 1 register.
3. Add support for memory access for arithmetic and logic operations, current instruction format supports it, but as of now, data has to loaded from memory to a register and then use it for calulation. 
4. Remove 8 bit data limitation for memory and registers. This could mean a redesign for instruction decode and result packing functions and making accomodations for it in the operations.
5. Support for signed integers, this could mean proper use of N (Negative) and V (Signed Oveflow) status register and improved scope for the system.
6. Seperate the operations in CPU class to something like ALU to just handle the operations and let the CPU class to just deal with instruction decode and result packing.