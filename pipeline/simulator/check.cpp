#include "check.h"

void checkStall() {
	STALL = false;
	if (ID_EX.pc_branch_out == 1) return;

	bool IDLoad = (ID_EX.opcode_out == LW || ID_EX.opcode_out == LH || ID_EX.opcode_out == LHU || ID_EX.opcode_out == LB || ID_EX.opcode_out == LBU);
	bool EXLoad = (EX_DM.opcode_out == LW || EX_DM.opcode_out == LH || EX_DM.opcode_out == LHU || EX_DM.opcode_out == LB || EX_DM.opcode_out == LBU);

	bool EXwriteReg = (EX_DM.opcode_out == R && EX_DM.funct_out != JR) || (EX_DM.opcode_out != R && EX_DM.opcode_out != HALT && EX_DM.opcode_out != J && EX_DM.opcode_out != BGTZ && EX_DM.opcode_out != BNE && EX_DM.opcode_out != BEQ && EX_DM.opcode_out != SB && EX_DM.opcode_out != SW && EX_DM.opcode_out != SH);
	bool IDwriteReg = (ID_EX.opcode_out == R && ID_EX.funct_out != JR) || (ID_EX.opcode_out != R && ID_EX.opcode_out != HALT && ID_EX.opcode_out != J && ID_EX.opcode_out != BGTZ && ID_EX.opcode_out != BNE && ID_EX.opcode_out != BEQ && ID_EX.opcode_out != SB && ID_EX.opcode_out != SW && ID_EX.opcode_out != SH);	

	// EX_DM to ID (not branches) stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	}
	// ID_EX to ID (branches) stall
	if (IF_ID.opcode_out == BEQ || IF_ID.opcode_out == BNE) {
		if (IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BGTZ) {
		if (IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && IF_ID.funct_out == JR) {
		if (IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	}
	// memory read stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = true;
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(IDwriteReg && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = true;
	} 
	else if (IF_ID.opcode_out == BNE || IF_ID.opcode_out == BEQ) {
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BGTZ || (IF_ID.opcode_out == R && IF_ID.funct_out == JR)) {
		if (EXLoad && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	}

	// memory read stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BNE || IF_ID.opcode_out == BEQ || IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = true;
	} 
	else if (IF_ID.opcode_out == BGTZ || (IF_ID.opcode_out == R && IF_ID.funct_out == JR)) {
		if (IDLoad && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = true;
	}
}

void checkForwardToID() {
	Forward::rs2ID = false;
	Forward::rt2ID = false;

	bool EXwriteReg = (EX_DM.opcode_out == R && EX_DM.funct_out != JR) || (EX_DM.opcode_out != R && EX_DM.opcode_out != HALT && EX_DM.opcode_out != J && EX_DM.opcode_out != BGTZ && EX_DM.opcode_out != BNE && EX_DM.opcode_out != BEQ && EX_DM.opcode_out != SB && EX_DM.opcode_out != SW && EX_DM.opcode_out != SH);
	if (IF_ID.opcode_out == BEQ || IF_ID.opcode_out == BNE) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {		
			if(EX_DM.reg_to_write_out == IF_ID.rt_out) Forward::rt2ID = true;
			if(EX_DM.reg_to_write_out == IF_ID.rs_out) Forward::rs2ID = true;
		}
	} 
	else if (IF_ID.opcode_out == BGTZ) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {
		 	if(EX_DM.reg_to_write_out == IF_ID.rs_out) Forward::rs2ID = true;
		}
	} 
	else if (IF_ID.opcode_out == R && IF_ID.funct_out == JR) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {
			if(EX_DM.reg_to_write_out == IF_ID.rs_out) Forward::rs2ID = true;
		}
	}
}

void checkForwardToEX() {
	Forward::rs2EX = false;
	Forward::rt2EX = false;

	bool EXwriteReg = (EX_DM.opcode_out == R && EX_DM.funct_out != JR) || (EX_DM.opcode_out != R && EX_DM.opcode_out != HALT && EX_DM.opcode_out != J && EX_DM.opcode_out != BGTZ && EX_DM.opcode_out != BNE && EX_DM.opcode_out != BEQ && EX_DM.opcode_out != SB && EX_DM.opcode_out != SW && EX_DM.opcode_out != SH);
	bool IDwriteReg = (ID_EX.opcode_out == R && ID_EX.funct_out != JR) || (ID_EX.opcode_out != R && ID_EX.opcode_out != HALT && ID_EX.opcode_out != J && ID_EX.opcode_out != BGTZ && ID_EX.opcode_out != BNE && ID_EX.opcode_out != BEQ && ID_EX.opcode_out != SB && ID_EX.opcode_out != SW && ID_EX.opcode_out != SH);	

	if(ID_EX.opcode_out == R) {
		if(ID_EX.funct_out == SLL || ID_EX.funct_out == SRL || ID_EX.funct_out == SRA) {
			if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {
				if(EX_DM.reg_to_write_out == ID_EX.rt_out) Forward::rt2EX = true;
			}
		}
		else if(ID_EX.funct_out != JR) {
			if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {
				if(EX_DM.reg_to_write_out == ID_EX.rs_out) Forward::rs2EX = true;
				if(EX_DM.reg_to_write_out == ID_EX.rt_out) Forward::rt2EX = true;
			}
		}
	}
	else if (ID_EX.opcode_out != R && ID_EX.opcode_out != LUI && ID_EX.opcode_out != BEQ && ID_EX.opcode_out != BNE && ID_EX.opcode_out != BGTZ
	&& ID_EX.opcode_out != J && ID_EX.opcode_out != JAL && ID_EX.opcode_out != HALT && ID_EX.opcode_out != SW && ID_EX.opcode_out != SH && ID_EX.opcode_out != SB) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {
			if(EX_DM.reg_to_write_out == ID_EX.rs_out) Forward::rs2EX = true;
		}
	} 
	else if (ID_EX.opcode_out == SW || ID_EX.opcode_out == SH || ID_EX.opcode_out == SB) {
		if (EXwriteReg && (EX_DM.reg_to_write_out != 0)) {
			if(EX_DM.reg_to_write_out == ID_EX.rs_out) Forward::rs2EX = true;
			if(EX_DM.reg_to_write_out == ID_EX.rt_out) Forward::rt2EX = true;
		}
		
	}
}