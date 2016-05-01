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
	static bool IF_halt, ID_halt, EX_halt, DM_halt, WB_halt;
	static bool write2Zero, numberOverflow, memoryOverflow, dataMisaaligned;
};

class Forward {
public:
	static bool rs2EX, rt2EX, rs2ID, rt2ID;
};

typedef struct _Buffer {
    unsigned inst_in, inst_out;
	unsigned pc_plus_four_in, pc_plus_four_out;

	unsigned op_in, op_out;
	unsigned func_in, func_out;
	unsigned shamt_in, shamt_out;
	unsigned rt_in, rt_out;
	unsigned rd_in, rd_out;
	unsigned rs_in, rs_out;

	unsigned reg_dst_in, reg_dst_out;
	unsigned alu_src_in, alu_src_out;

	bool mem_read_in, mem_read_out, mem_write_in, mem_write_out;
	unsigned mem_op_in, mem_op_out;

	unsigned reg_write_in, reg_write_out;
	unsigned mem_to_reg_in, mem_to_reg_out;

	unsigned $rs_in, $rs_out;
	unsigned $rt_in, $rt_out;
	unsigned extended_imm_in, extended_imm_out;

	bool pc_src_in, pc_src_out;
	//bool needToStall;
	unsigned pc_in, pc_out;
	unsigned reg_to_write_in, reg_to_write_out;

	unsigned write_data_in, write_data_out;
	unsigned alu_result_in, alu_result_out;
	unsigned read_data_in, read_data_out;
}Buffer;

extern bool stall;

extern Buffer IF_ID;
extern Buffer ID_EX;
extern Buffer EX_DM;
extern Buffer DM_WB;
extern Buffer WB_END;

#endif
