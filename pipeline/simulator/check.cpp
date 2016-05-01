#include "check.h"

void checkStall() {
	stall = false;
	if (ID_EX.pc_src_out == 1) return;
	// EX_DM to ID (not branches) stall
	if (IF_ID.op_out == R && IF_ID.func_out != SLL && IF_ID.func_out != SRL && IF_ID.func_out != SRA && IF_ID.func_out != JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) stall = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) stall = true;
	} else if (IF_ID.op_out == R && (IF_ID.func_out == SLL || IF_ID.func_out == SRL || IF_ID.func_out == SRA)) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) stall = true;
	} else if (IF_ID.op_out != R && IF_ID.op_out != LUI && IF_ID.op_out != BEQ && IF_ID.op_out != BNE && IF_ID.op_out != BGTZ
	&& IF_ID.op_out != J && IF_ID.op_out != JAL && IF_ID.op_out != HALT && IF_ID.op_out != SW && IF_ID.op_out != SH && IF_ID.op_out != SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) stall = true;
	} else if (IF_ID.op_out == SW || IF_ID.op_out == SH || IF_ID.op_out == SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) stall = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) stall = true;
	}
	// ID_EX to ID (branches) stall
	if (IF_ID.op_out == BEQ || IF_ID.op_out == BNE) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) stall = true;
	} else if (IF_ID.op_out == BGTZ) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
	} else if (IF_ID.op_out == R && IF_ID.func_out == JR) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
	}
	// memory read stall
	if (IF_ID.op_out == R && IF_ID.func_out != SLL && IF_ID.func_out != SRL && IF_ID.func_out != SRA && IF_ID.func_out != JR) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) stall = true;
	} else if (IF_ID.op_out == R && (IF_ID.func_out == SLL || IF_ID.func_out == SRL || IF_ID.func_out == SRA)) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) stall = true;
	} else if (IF_ID.op_out != R && IF_ID.op_out != LUI && IF_ID.op_out != BEQ && IF_ID.op_out != BNE && IF_ID.op_out != BGTZ
	&& IF_ID.op_out != J && IF_ID.op_out != JAL && IF_ID.op_out != HALT && IF_ID.op_out != SW && IF_ID.op_out != SH && IF_ID.op_out != SB) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
	} else if (IF_ID.op_out == BNE || IF_ID.op_out == BEQ || IF_ID.op_out == SW || IF_ID.op_out == SH || IF_ID.op_out == SB) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) stall = true;
	} else if (IF_ID.op_out == BGTZ || (IF_ID.op_out == R && IF_ID.func_out == JR)) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) stall = true;
	}
	// memory read stall
	if (IF_ID.op_out == R && IF_ID.func_out != SLL && IF_ID.func_out != SRL && IF_ID.func_out != SRA && IF_ID.func_out != JR) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) stall = true;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) stall = true;
	} else if (IF_ID.op_out == R && (IF_ID.func_out == SLL || IF_ID.func_out == SRL || IF_ID.func_out == SRA)) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) stall = true;
	} else if (IF_ID.op_out != R && IF_ID.op_out != LUI && IF_ID.op_out != BEQ && IF_ID.op_out != BNE && IF_ID.op_out != BGTZ
	&& IF_ID.op_out != J && IF_ID.op_out != JAL && IF_ID.op_out != HALT && IF_ID.op_out != SW && IF_ID.op_out != SH && IF_ID.op_out != SB) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) stall = true;
	} else if (IF_ID.op_out == SW || IF_ID.op_out == SH || IF_ID.op_out == SB) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) stall = true;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) stall = true;
	} else if (IF_ID.op_out == BNE || IF_ID.op_out == BEQ) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) stall = true;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out)) stall = true;
	} else if (IF_ID.op_out == BGTZ || (IF_ID.op_out == R && IF_ID.func_out == JR)) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) stall = true;
	}
}

void checkForwarding() {
	Forward::rs2ID = false;
	Forward::rt2ID = false;
	if (IF_ID.op_out == BEQ || IF_ID.op_out == BNE) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) Forward::rs2ID = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out)) Forward::rt2ID = true;
	} 
	else if (IF_ID.op_out == BGTZ) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) Forward::rs2ID = true;
	} 
	else if (IF_ID.op_out == R && IF_ID.func_out == JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) Forward::rs2ID = true;
	}

	Forward::rs2EX = false;
	Forward::rt2EX = false;
	if (ID_EX.op_out == R && ID_EX.func_out != SLL && ID_EX.func_out != SRL && ID_EX.func_out != SRA && ID_EX.func_out != JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rs_out)) Forward::rs2EX = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rt_out)) Forward::rt2EX = true;
	} 
	else if (ID_EX.op_out == R && (ID_EX.func_out == SLL || ID_EX.func_out == SRL || ID_EX.func_out == SRA)) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rt_out)) Forward::rt2EX = true;
	} 
	else if (ID_EX.op_out != R && ID_EX.op_out != LUI && ID_EX.op_out != BEQ && ID_EX.op_out != BNE && ID_EX.op_out != BGTZ
	&& ID_EX.op_out != J && ID_EX.op_out != JAL && ID_EX.op_out != HALT && ID_EX.op_out != SW && ID_EX.op_out != SH && ID_EX.op_out != SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rs_out)) Forward::rs2EX = true;
	} 
	else if (ID_EX.op_out == SW || ID_EX.op_out == SH || ID_EX.op_out == SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rs_out)) Forward::rs2EX = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rt_out)) Forward::rt2EX = true;
	}
}
