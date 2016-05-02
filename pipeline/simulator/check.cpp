#include "check.h"

void checkStall() {
	STALL = false;
	if (ID_EX.pc_src_out == 1) return;
	// EX_DM to ID (not branches) stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	}
	// ID_EX to ID (branches) stall
	if (IF_ID.opcode_out == BEQ || IF_ID.opcode_out == BNE) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BGTZ) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && IF_ID.funct_out == JR) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	}
	// memory read stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == BNE || IF_ID.opcode_out == BEQ) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BGTZ || (IF_ID.opcode_out == R && IF_ID.funct_out == JR)) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	}

	// memory read stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BNE || IF_ID.opcode_out == BEQ || IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BGTZ || (IF_ID.opcode_out == R && IF_ID.funct_out == JR)) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	}
}

void checkForwarding() {
	//checkFwdToEx
	Forward::rs2EX = false;
	Forward::rt2EX = false;
	if(ID_EX.opcode_out == R) {
		if(ID_EX.funct_out == SLL || ID_EX.funct_out == SRL || ID_EX.funct_out == SRA) {
			if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {
				if(EX_DM.reg_to_write_out == ID_EX.rt_out) Forward::rt2EX = true;
			}
		}
		else if(ID_EX.funct_out != JR) {
			if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {
				if(EX_DM.reg_to_write_out == ID_EX.rs_out) Forward::rs2EX = true;
				if(EX_DM.reg_to_write_out == ID_EX.rt_out) Forward::rt2EX = true;
			}
		}
	}
	else if (ID_EX.opcode_out != R && ID_EX.opcode_out != LUI && ID_EX.opcode_out != BEQ && ID_EX.opcode_out != BNE && ID_EX.opcode_out != BGTZ
	&& ID_EX.opcode_out != J && ID_EX.opcode_out != JAL && ID_EX.opcode_out != HALT && ID_EX.opcode_out != SW && ID_EX.opcode_out != SH && ID_EX.opcode_out != SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {
			if(EX_DM.reg_to_write_out == ID_EX.rs_out) Forward::rs2EX = true;
		}
	} 
	else if (ID_EX.opcode_out == SW || ID_EX.opcode_out == SH || ID_EX.opcode_out == SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {
			if(EX_DM.reg_to_write_out == ID_EX.rs_out) Forward::rs2EX = true;
			if(EX_DM.reg_to_write_out == ID_EX.rt_out) Forward::rt2EX = true;
		}
		
	}

	//checkFwdToId
	Forward::rs2ID = false;
	Forward::rt2ID = false;
	if (IF_ID.opcode_out == BEQ || IF_ID.opcode_out == BNE) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {		
			if(EX_DM.reg_to_write_out == IF_ID.rt_out) Forward::rt2ID = true;
			if(EX_DM.reg_to_write_out == IF_ID.rs_out) Forward::rs2ID = true;
		}
	} 
	else if (IF_ID.opcode_out == BGTZ) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {
		 	if(EX_DM.reg_to_write_out == IF_ID.rs_out) Forward::rs2ID = true;
		}
	} 
	else if (IF_ID.opcode_out == R && IF_ID.funct_out == JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0)) {
			if(EX_DM.reg_to_write_out == IF_ID.rs_out) Forward::rs2ID = true;
		}
	}

}
