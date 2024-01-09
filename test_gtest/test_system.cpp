#include <gtest/gtest.h>
#include "../src/cpu.h"

// Test MOV.
TEST(TestSystem, MOV) {
    static Cpu cpu;
    //MOV R0 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011003ff), 0xff00000000000000);
    //MOV R1 0xEE
    EXPECT_EQ(cpu.executeOpDump(0x011013ee), 0xffee000000000000);
    //MOV R2 0xdd
    EXPECT_EQ(cpu.executeOpDump(0x011023dd), 0xffeedd0000000000);
    //MOV R3 0xcc
    EXPECT_EQ(cpu.executeOpDump(0x011033cc), 0xffeeddcc00000000);
    //MOV R0 R3
    EXPECT_EQ(cpu.executeOpDump(0x01100103), 0xcceeddcc00000000);
    //MOV R3 R1
    EXPECT_EQ(cpu.executeOpDump(0x01103101), 0xcceeddee00000000);
    //MOV R1 R2
    EXPECT_EQ(cpu.executeOpDump(0x01101102), 0xccddddee00000000);
    //MOV R2 R0
    EXPECT_EQ(cpu.executeOpDump(0x01102100), 0xccddccee00000000);
}

TEST(TestSystem, ADD) {
    static Cpu cpu;
    //MOV R0 0x64
    EXPECT_EQ(cpu.executeOpDump(0x01100364), 0x6400000000000000);
    //MOV R1 0x32
    EXPECT_EQ(cpu.executeOpDump(0x01101332), 0x6432000000000000);
    //ADD R0 R1
    EXPECT_EQ(cpu.executeOpDump(0x02100101), 0x9632000000000000);
    //MOV R2 0x64
    EXPECT_EQ(cpu.executeOpDump(0x01102364), 0x9632640000000000);
    //ADD R2 0x32
    EXPECT_EQ(cpu.executeOpDump(0x02102332), 0x9632960000000000);
}

TEST(TestSystem, SUB) {
    static Cpu cpu;
    //MOV R3 0x64
    EXPECT_EQ(cpu.executeOpDump(0x01103364), 0x0000006400000000);
    //MOV R2 0x32
    EXPECT_EQ(cpu.executeOpDump(0x01102332), 0x0000326400000000);
    //SUB R3 R2
    EXPECT_EQ(cpu.executeOpDump(0x03103102), 0x0000323200000000);
    //MOV R1 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011013ff), 0x00ff323200000000);
    //SUB R1 0x32
    EXPECT_EQ(cpu.executeOpDump(0x02101332), 0x0031323200000000);
}

TEST(TestSystem, CMP) {
    static Cpu cpu;
    //MOV R0 0x64
    EXPECT_EQ(cpu.executeOpDump(0x01100364), 0x6400000000000000);
    //MOV R1 0x32
    EXPECT_EQ(cpu.executeOpDump(0x01101332), 0x6432000000000000);
    //CMP R0 R1 // Negative Flag will be set since OP1 > OP2
    EXPECT_EQ(cpu.executeOpDump(0x04100101), 0x6432000000000001);
    //CMP R1 R0 // V Flag will be set since OP1 < OP2 // Design Choice
    EXPECT_EQ(cpu.executeOpDump(0x04101100), 0x6432000001000000);
    //CMP R0 R0 // Zero Flag will be set since OP1 == OP2
    EXPECT_EQ(cpu.executeOpDump(0x04100100), 0x6432000000010000);
}

TEST(TestSystem, JMP) {
    // TODO: Will have to decide to, probably run factorial/fibonacci
}

TEST(TestSystem, LOD_STR) { // Stores and Loads from memory location specified as offset
    static Cpu cpu;
    //MOV R0 0xAB
    EXPECT_EQ(cpu.executeOpDump(0x011003ab), 0xab00000000000000);
    //STR R0 0x32 // Store data in R0 to @offset0x32
    EXPECT_EQ(cpu.executeOpDump(0x07100232), 0xab00000000000000);
    //LOD R1 0x32 // Load data in @osffset0x32 to R1
    EXPECT_EQ(cpu.executeOpDump(0x06101232), 0xabab000000000000);
    //MOV R3 0xCD
    EXPECT_EQ(cpu.executeOpDump(0x011033cd), 0xabab00cd00000000);
    //STR R3 0xff // Store data in R0 to @offset0x32
    EXPECT_EQ(cpu.executeOpDump(0x071032ff), 0xabab00cd00000000);
    //LOD R2 0x32 // Load data in @osffset0x32 to R1
    EXPECT_EQ(cpu.executeOpDump(0x061022ff), 0xababcdcd00000000);
}

TEST(TestSystem, MUL) {
    static Cpu cpu;
    //MOV R0 0x16
    EXPECT_EQ(cpu.executeOpDump(0x01100316), 0x1600000000000000);
    //MOV R1 0x32
    EXPECT_EQ(cpu.executeOpDump(0x01101332), 0x1632000000000000);
    //MUL R0 R1 // 0x16 * 0x32 = 0x44C (out of capacity), hence trimmed to available size and carry flag set
    EXPECT_EQ(cpu.executeOpDump(0x08100101), 0x4c32000000000100);
    //MOV R3 0x0F
    EXPECT_EQ(cpu.executeOpDump(0x0110330f), 0x4c32000f00000100);
    //MUL R3 0x0A // 0xF * 0xA = 0x96
    EXPECT_EQ(cpu.executeOpDump(0x0810330a), 0x4c32009600000000);
}

TEST(TestSystem, DIV) { // System does not support float 
    static Cpu cpu;
    //MOV R0 0x32
    EXPECT_EQ(cpu.executeOpDump(0x01100332), 0x3200000000000000);
    //MOV R1 0x19
    EXPECT_EQ(cpu.executeOpDump(0x01101319), 0x3219000000000000);
    //DIV R0 R1 // 0x32 / 0x19 = 0x2 
    EXPECT_EQ(cpu.executeOpDump(0x09100101), 0x0219000000000000);
    //MOV R3 0x10
    EXPECT_EQ(cpu.executeOpDump(0x01103310), 0x0219001000000000);
    //DIV R3 0x0 // 0x10 / 0x0 = UNDEFINED (Division by Zero) V flag set
    EXPECT_EQ(cpu.executeOpDump(0x09103300), 0x219001000000001);
}

TEST(TestSystem, LSH) { 
    static Cpu cpu;
    //MOV R0 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011003ff), 0xff00000000000000);
    //MOV R1 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011013ff), 0xffff000000000000);
    //LSH R1 0x4
    EXPECT_EQ(cpu.executeOpDump(0x0a101304), 0xfff0000000000000);
    //MOV R2 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011023ff), 0xfff0ff0000000000);
    //MOV R3 0x4
    EXPECT_EQ(cpu.executeOpDump(0x01103304), 0xfff0ff0400000000);
    //LSH R2 R3
    EXPECT_EQ(cpu.executeOpDump(0x0a102103), 0xfff0f00400000000);
}

TEST(TestSystem, RSH) { 
    static Cpu cpu;
    //MOV R0 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011003ff), 0xff00000000000000);
    //MOV R1 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011013ff), 0xffff000000000000);
    //RSH R1 0x4
    EXPECT_EQ(cpu.executeOpDump(0x0b101304), 0xff0f000000000000);
    //MOV R2 0xFF
    EXPECT_EQ(cpu.executeOpDump(0x011023ff), 0xff0fff0000000000);
    //MOV R3 0x4
    EXPECT_EQ(cpu.executeOpDump(0x01103304), 0xff0fff0400000000);
    //LSH R2 R3
    EXPECT_EQ(cpu.executeOpDump(0x0b102103), 0xff0f0f0400000000);
}

TEST(TestSystem, AND) { 
    static Cpu cpu;
    //MOV R0 0xAA (10101010)
    EXPECT_EQ(cpu.executeOpDump(0x011003aa), 0xaa00000000000000);
    //MOV R1 0x55 (01010101)
    EXPECT_EQ(cpu.executeOpDump(0x01101355), 0xaa55000000000000);
    //AND R0 0x55 // Zero Flags Set since the result is 0
    EXPECT_EQ(cpu.executeOpDump(0x0c100355), 0x0055000000010000);
    //AND R1 0xAA // Zero Flags Set since tht result is 0
    EXPECT_EQ(cpu.executeOpDump(0x0c1013aa), 0x0000000000010000);
    //MOV R2 0xAA
    EXPECT_EQ(cpu.executeOpDump(0x011023aa), 0x0000aa0000010000);
    //MOV R3 0xF5
    EXPECT_EQ(cpu.executeOpDump(0x011033f5), 0x0000aaf500010000);
    //AND R2 R3
    EXPECT_EQ(cpu.executeOpDump(0x0c102103), 0x0000a0f500000000);
}

TEST(TestSystem, OR) { 
    static Cpu cpu;
    //MOV R0 0xAA (10101010)
    EXPECT_EQ(cpu.executeOpDump(0x011003aa), 0xaa00000000000000);
    //MOV R1 0x55 (01010101)
    EXPECT_EQ(cpu.executeOpDump(0x01101355), 0xaa55000000000000);
    //OR R0 0x55
    EXPECT_EQ(cpu.executeOpDump(0x0d100355), 0xff55000000000000);
    //OR R1 0xAA 
    EXPECT_EQ(cpu.executeOpDump(0x0d1013aa), 0xffff000000000000);
    //MOV R2 0xAA
    EXPECT_EQ(cpu.executeOpDump(0x011023aa), 0xffffaa0000000000);
    //MOV R3 0xF5
    EXPECT_EQ(cpu.executeOpDump(0x011033f5), 0xffffaaf500000000);
    //OR R2 R3
    EXPECT_EQ(cpu.executeOpDump(0x0d102103), 0xfffffff500000000);
}

TEST(TestSystem, XOR) { 
    static Cpu cpu;
    //MOV R0 0xAA (10101010)
    EXPECT_EQ(cpu.executeOpDump(0x011003aa), 0xaa00000000000000);
    //MOV R1 0x55 (01010101)
    EXPECT_EQ(cpu.executeOpDump(0x01101355), 0xaa55000000000000);
    //XOR R0 R1
    EXPECT_EQ(cpu.executeOpDump(0x0e100101), 0xff55000000000000);
    //MOV R2 0xAA
    EXPECT_EQ(cpu.executeOpDump(0x011023aa), 0xff55aa0000000000);
    //MOV R3 0xAA
    EXPECT_EQ(cpu.executeOpDump(0x011033aa), 0xff55aaaa00000000);
    //XOR R2 R3
    EXPECT_EQ(cpu.executeOpDump(0x0e102103), 0xff5500aa00010000);
}

TEST(TestSystem, NOT) { 
    static Cpu cpu;
    //MOV R0 0xAA (10101010)
    EXPECT_EQ(cpu.executeOpDump(0x011003aa), 0xaa00000000000000);
    //MOV R1 0x55 (01010101)
    EXPECT_EQ(cpu.executeOpDump(0x01101355), 0xaa55000000000000);
    //MOV R2 0xCC (11001100)
    EXPECT_EQ(cpu.executeOpDump(0x011023cc), 0xaa55cc0000000000);
    //MOV R3 0x33 (00110011)
    EXPECT_EQ(cpu.executeOpDump(0x01103333), 0xaa55cc3300000000);
    //NOT R0
    EXPECT_EQ(cpu.executeOpDump(0x0f100000), 0x5555cc3300000000);
    //NOT R1
    EXPECT_EQ(cpu.executeOpDump(0x0f101000), 0x55aacc3300000000);
    //NOT R2
    EXPECT_EQ(cpu.executeOpDump(0x0f102000), 0x55aa333300000000);
    //NOT R3
    EXPECT_EQ(cpu.executeOpDump(0x0f103000), 0x55aa33cc00000000);
}

TEST(TestSystem, HLT) {
    // TODO: Will have to decide to, Maybe just run, but doesn't prove anything.
}

TEST(TestSystem, NOP) {
    // TODO: Will have to decide to, Same as above, but can't validate anythign here.
}

