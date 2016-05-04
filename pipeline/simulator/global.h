#ifndef global_h
#define global_h

#include <stdio.h>
#include <stdlib.h>

class Register {
public:
	static unsigned reg[32], PC, cycle;
};

class Memory {
public:
	static unsigned char DMemory[1024], IMemory[1024];
	static unsigned address, position;
};

class Instruction {
public:
	static unsigned op, rs, rt, rd, func, shamt, immediate;
};

class Terminal {
public:
	static bool IF_HALT, ID_HALT, EX_HALT, DM_HALT, WB_HALT;
	//static bool halt;
	static bool write2Zero, numberOverflow, memoryOverflow, dataMisaaligned;
};

class Forward {
public:
	static bool rs2EX, rt2EX, rs2ID, rt2ID;
};

typedef struct _Buffer {
    unsigned ins_reg_in, ins_reg_out;
	unsigned pc_plus_four_in, pc_plus_four_out;

	unsigned opcode_in, opcode_out;
	unsigned funct_in, funct_out;
	unsigned shamt_in, shamt_out;
	unsigned rt_in, rt_out;
	unsigned rd_in, rd_out;
	unsigned rs_in, rs_out;

	unsigned alu_src_in, alu_src_out;

	unsigned $rs_in, $rs_out;
	unsigned $rt_in, $rt_out;
	unsigned extended_imme_in, extended_imme_out;

	bool pc_src_in, pc_src_out;

	unsigned pc_in, pc_out;
	unsigned reg_to_write_in, reg_to_write_out;

	unsigned write_data_in, write_data_out;
	unsigned alu_result_in, alu_result_out;
	unsigned read_data_in, read_data_out;
	
}Buffer;

extern bool STALL;

extern Buffer IF_ID;
extern Buffer ID_EX;
extern Buffer EX_DM;
extern Buffer DM_WB;

#endif
