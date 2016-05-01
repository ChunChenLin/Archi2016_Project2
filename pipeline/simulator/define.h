#ifndef DEFINE_H
#define DEFINE_H

#define ERROR 0xffffffff

#define R      0
#define ADD   32
#define ADDU  33
#define SUB   34
#define AND   36
#define OR    37
#define XOR   38
#define NOR   39
#define NAND  40
#define SLT   42
#define SLL    0
#define SRL    2
#define SRA    3
#define JR     8

#define ADDI   8
#define ADDIU  9
#define LW    35
#define LH    33
#define LHU   37
#define LB    32
#define LBU   36
#define SW    43
#define SH    41
#define SB    40
#define LUI   15
#define ANDI  12
#define ORI   13
#define NORI  14
#define SLTI  10
#define BEQ    4
#define BNE    5
#define BGTZ   7
#define J      2
#define JAL    3
#define HALT  63

#define MEM_WORD 0
#define MEM_HALF 1
#define MEM_HALF_UNSIGN 2
#define MEM_BYTE 3
#define MEM_BYTE_UNSIGN 4

#endif