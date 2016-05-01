#include "stages.h"
/*
inline void detectWrite2Zero(string format) {
	if(format=="R") {
		if(Instruction::rd==0) {
			Terminal::write2Zero = true;
			Register::reg[Instruction::rd] = 0;
		}
	} else { //I format
		if(Instruction::rt==0) {
			Terminal::write2Zero = true;
			Register::reg[Instruction::rt] = 0;
		}
	}
}
*/
inline void detectNumberOverflow(int a, int b, int c) { 
	if((a>0&&b>0&&c<0) || (a<0&&b<0&&c>0)) {
		Terminal::numberOverflow = true;
	} 
}
/*
inline bool detectMemoryOverflow(int n) {
	if(Memory::position + n >= 1024 || Memory::position >= 1024) {
		Terminal::memoryOverflow = true;
		Terminal::halt = true;
		return true;
	} else return false;
}

inline bool detectDataMisaaligned(int n) {
	if(Memory::position % (n+1) > 0) {
		Terminal::dataMisaaligned = true;
		Terminal::halt = true;
		return true;
	} else return false;
}
*/
void IF() {
	if (STALL) return;
	if (ID_EX.pc_src_out == 1) Register::PC = ID_EX.pc_out;
	else Register::PC = IF_ID.pc_plus_four_out;

	for (int i = 0; i < 4; i++)
		IF_ID.ins_reg_in = (IF_ID.ins_reg_in << 8) | (unsigned char) Memory::IMemory[Register::PC + i];
	Register::PC = Register::PC + 4;
	IF_ID.pc_plus_four_in = Register::PC;

	IF_ID.opcode_in = IF_ID.ins_reg_in >> 26;
	IF_ID.funct_in = IF_ID.ins_reg_in << 26 >> 26;
	IF_ID.rs_in = IF_ID.ins_reg_in << 6 >> 27;
	IF_ID.rt_in = IF_ID.ins_reg_in << 11 >> 27;
}

void Control() {
	if (ID_EX.opcode_in == R) {
		ID_EX.reg_dst_in = 1;
		ID_EX.alu_src_in = 0;
	} else if (ID_EX.opcode_in == JAL) {
		ID_EX.reg_dst_in = 2;
		ID_EX.alu_src_in = 1;
	} else {
		ID_EX.reg_dst_in = 0;
		ID_EX.alu_src_in = 1;
	}

	if (ID_EX.opcode_in == LW || ID_EX.opcode_in == LH || ID_EX.opcode_in == LHU || ID_EX.opcode_in == LB || ID_EX.opcode_in == LBU) ID_EX.mem_read_in = 1;
	else ID_EX.mem_read_in = 0;
	
	if (ID_EX.opcode_in == SW || ID_EX.opcode_in == SH || ID_EX.opcode_in == SB) ID_EX.mem_write_in = 1;
	else ID_EX.mem_write_in = 0;
	
	if (ID_EX.opcode_in == LW || ID_EX.opcode_in == SW) ID_EX.mem_op_in = MEM_WORD;
	else if (ID_EX.opcode_in == LH || ID_EX.opcode_in == SH) ID_EX.mem_op_in = MEM_HALF;
	else if (ID_EX.opcode_in == LB || ID_EX.opcode_in == SB) ID_EX.mem_op_in = MEM_BYTE;
	else if (ID_EX.opcode_in == LHU) ID_EX.mem_op_in = MEM_HALF_UNSIGN;
	else if (ID_EX.opcode_in == LBU) ID_EX.mem_op_in = MEM_BYTE_UNSIGN;

	if (ID_EX.opcode_in == R) {
		if (ID_EX.funct_in != JR) ID_EX.reg_write_in = 1;
		else ID_EX.reg_write_in = 0;
	} 
	else if (ID_EX.opcode_in == ADDI || ID_EX.opcode_in == ADDIU || ID_EX.opcode_in == LW || ID_EX.opcode_in == LH || ID_EX.opcode_in == LHU || ID_EX.opcode_in == LB
	|| ID_EX.opcode_in == LBU || ID_EX.opcode_in == LUI || ID_EX.opcode_in == ANDI || ID_EX.opcode_in == ORI || ID_EX.opcode_in == NORI || ID_EX.opcode_in == SLTI || ID_EX.opcode_in == JAL) {
		ID_EX.reg_write_in = 1;
	} 
	else ID_EX.reg_write_in = 0;
	
	if (ID_EX.opcode_in == LW || ID_EX.opcode_in == LH || ID_EX.opcode_in == LHU || ID_EX.opcode_in == LB || ID_EX.opcode_in == LBU) ID_EX.mem_to_reg_in = 1;
	else ID_EX.mem_to_reg_in = 0;
}


void ID() {

	if (STALL) return;
	if (ID_EX.pc_src_out) {
		ID_EX.pc_plus_four_in = 0;
		ID_EX.opcode_in = 0;
		ID_EX.funct_in = 0;
		ID_EX.shamt_in = 0;
		ID_EX.rt_in = 0;
		ID_EX.rd_in = 0;
		ID_EX.rs_in = 0;

		ID_EX.reg_dst_in = 0;
		ID_EX.alu_src_in = 0;

		ID_EX.mem_read_in = 0;
		ID_EX.mem_write_in = 0;
		ID_EX.mem_op_in = 0;

		ID_EX.reg_write_in = 0;
		ID_EX.mem_to_reg_in = 0;

		ID_EX.$rs_in = 0;
		ID_EX.$rt_in = 0;
		ID_EX.extended_imme_in = 0;

		ID_EX.pc_src_in = 0;
		ID_EX.pc_in = 0;
		ID_EX.reg_to_write_in = 0;
		return;
	}
	/*
	ID_EX.pc_plus_four_in = IF_ID.pc_plus_four_out;
	ID_EX.opcode_in = IF_ID.opcode_out;
	ID_EX.funct_in = IF_ID.funct_out;
	ID_EX.shamt_in = IF_ID.ins_reg_out << 21 >> 27;
	ID_EX.rs_in = IF_ID.rs_out;
	ID_EX.rt_in = IF_ID.rt_out;
	ID_EX.rd_in = IF_ID.ins_reg_out << 16 >> 27;
	*/

	unsigned address = IF_ID.ins_reg_out << 6 >> 6;

	Control();

	if (Forward::rs2ID) ID_EX.$rs_in = EX_DM.alu_result_out;
	else ID_EX.$rs_in = Register::reg[ID_EX.rs_in];
	if (Forward::rt2ID) ID_EX.$rt_in = EX_DM.alu_result_out;
	else ID_EX.$rt_in = Register::reg[ID_EX.rt_in];
	short temp = IF_ID.ins_reg_out << 16 >> 16;
	ID_EX.extended_imme_in = temp;

	if (ID_EX.reg_dst_in == 0) ID_EX.reg_to_write_in = ID_EX.rt_in;
	else if (ID_EX.reg_dst_in == 1) ID_EX.reg_to_write_in = ID_EX.rd_in;
	else if (ID_EX.reg_dst_in == 2) ID_EX.reg_to_write_in = 31;
	else printf("ID_EX.reg_dst_in error.\n");

	if (ID_EX.opcode_in == BEQ) {
		if (ID_EX.$rs_in == ID_EX.$rt_in) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (ID_EX.opcode_in == BNE) {
		if (ID_EX.$rs_in != ID_EX.$rt_in) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (ID_EX.opcode_in == BGTZ) {
		int intRs = ID_EX.$rs_in;
		if (intRs > 0) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (ID_EX.opcode_in == J || ID_EX.opcode_in == JAL) {
		ID_EX.pc_src_in = 1;
		ID_EX.pc_in = (IF_ID.pc_plus_four_out >> 28 << 28) | (address << 2);
	} else if (ID_EX.opcode_in == R && ID_EX.funct_in == JR) {
		ID_EX.pc_src_in = 1;
		ID_EX.pc_in = ID_EX.$rs_in;
	} else ID_EX.pc_src_in = 0;

}

void EX() {

	/*
	EX_DM.opcode_in = ID_EX.opcode_out;
	EX_DM.rd_in = ID_EX.rd_out;
	EX_DM.rt_in = ID_EX.rt_out;
	EX_DM.rs_in = ID_EX.rs_out;
	EX_DM.funct_in = ID_EX.funct_out;
	EX_DM.shamt_in = ID_EX.shamt_out;

	EX_DM.mem_read_in = ID_EX.mem_read_out;
	EX_DM.mem_write_in = ID_EX.mem_write_out;
	EX_DM.mem_op_in = ID_EX.mem_op_out;

	EX_DM.reg_write_in = ID_EX.reg_write_out;
	EX_DM.mem_to_reg_in = ID_EX.mem_to_reg_out;

	EX_DM.$rs_in = ID_EX.$rs_out;
	EX_DM.$rt_in = ID_EX.$rt_out;
	EX_DM.reg_to_write_in = ID_EX.reg_to_write_out;
	*/

	unsigned left, right;
	if (EX_DM.opcode_in == SW || EX_DM.opcode_in == SH || EX_DM.opcode_in == SB) {
		if (Forward::rs2EX) left = EX_DM.alu_result_out;
		else left = ID_EX.$rs_out;
		right = ID_EX.extended_imme_out;
		if (Forward::rt2EX) EX_DM.write_data_in = EX_DM.alu_result_out;
		else EX_DM.write_data_in = ID_EX.$rt_out;
	} else {
		if (Forward::rs2EX) left = EX_DM.alu_result_out;
		else left = ID_EX.$rs_out;
		if (ID_EX.alu_src_out == 0) {
			if (Forward::rt2EX) right = EX_DM.alu_result_out;
			else right = ID_EX.$rt_out;
		} else right = ID_EX.extended_imme_out;
	}
	unsigned left_sign; // = left >> 31;
	unsigned right_sign;// = right >> 31;
	unsigned result_sign;
	signed int Ileft = (int)left;
	signed int Iright = (int)right;
	signed int Iresult = Ileft + Iright;
	switch (EX_DM.opcode_in) {
		case R:
			switch (EX_DM.funct_in) {
				case ADD:
					left_sign = left >> 31, right_sign = right >> 31;
					EX_DM.alu_result_in = left + right;
					result_sign = EX_DM.alu_result_in >> 31;
					if (left_sign == right_sign && left_sign != result_sign) {
						Terminal::numberOverflow = true;
					}
					break;
				case ADDU:
					EX_DM.alu_result_in = left + right;
					break;
				case SUB:
					left_sign = left >> 31, right_sign = (-right) >> 31;
					EX_DM.alu_result_in = left - right;
					result_sign = EX_DM.alu_result_in >> 31;
					if (left_sign == right_sign && left_sign != result_sign) {
						Terminal::numberOverflow = true;
					}
					break;
				case AND:
					EX_DM.alu_result_in = left & right;
					break;
				case OR:
					EX_DM.alu_result_in = left | right;
					break;
				case XOR:
					EX_DM.alu_result_in = left ^ right;
					break;
				case NOR:
					EX_DM.alu_result_in = ~(left | right);
					break;
				case NAND:
					EX_DM.alu_result_in = ~(left & right);
					break;
				case SLT:
					//intRight = right, intLeft = left;
					EX_DM.alu_result_in = ((int)left < (int)right) ? 1 : 0;
					break;
				case SLL:
					EX_DM.alu_result_in = right << EX_DM.shamt_in;
					break;
				case SRL:
					EX_DM.alu_result_in = right >> EX_DM.shamt_in;
					break;
				case SRA:
					//temp = right;
					EX_DM.alu_result_in = (int)right >> EX_DM.shamt_in;
					break;
				default:
					break;
			}
			break;
		case ADDI:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			//Iresult = Ileft+ Iright;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case ADDIU:
			EX_DM.alu_result_in = left + right;
			break;
		case LW:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case LH:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case LHU:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case LB:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case LBU:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case SW:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case SH:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case SB:
			//Ileft = (int)left, Iright = (int)right;
			EX_DM.alu_result_in = left + right;
			//Iresult = Ileft+ Iright;
			//result_sign = EX_DM.alu_result_in >> 31;
			//if (left_sign == right_sign && left_sign != result_sign) Terminal::numberOverflow = 1;
			detectNumberOverflow(Ileft,Iright,Iresult);
			break;
		case LUI:
			EX_DM.alu_result_in = right << 16;
			break;
		case ANDI:
			EX_DM.alu_result_in = left & (unsigned short) right;
			break;
		case ORI:
			EX_DM.alu_result_in = left | (unsigned short) right;
			break;
		case NORI:
			EX_DM.alu_result_in = ~(left | (unsigned short) right);
			break;
		case SLTI:
			//intRight = right, intLeft = left;
			EX_DM.alu_result_in = ((int)left < (int)right) ? 1 : 0;
			break;
		case JAL:
			EX_DM.alu_result_in = ID_EX.pc_plus_four_out;
		default:
			break;
	}

}

void DM() {
	/*
	DM_WB.opcode_in = EX_DM.opcode_out;
	DM_WB.rd_in = EX_DM.rd_out;
	DM_WB.rt_in = EX_DM.rt_out;
	DM_WB.rs_in = EX_DM.rs_out;
	DM_WB.shamt_in = EX_DM.shamt_out;
	DM_WB.funct_in = EX_DM.funct_out;

	DM_WB.reg_write_in = EX_DM.reg_write_out;
	DM_WB.mem_to_reg_in = EX_DM.mem_to_reg_out;

	DM_WB.alu_result_in = EX_DM.alu_result_out;
	DM_WB.reg_to_write_in = EX_DM.reg_to_write_out;
	*/

	if (EX_DM.mem_write_out == 1) {
		if (EX_DM.mem_op_out == MEM_WORD) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 3 >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (EX_DM.alu_result_out % 4) {
				Terminal::dataMisaaligned = 1;
			}
			if (Terminal::memoryOverflow || Terminal::dataMisaaligned) return;
			Memory::DMemory[EX_DM.alu_result_out] = EX_DM.write_data_out >> 24;
			Memory::DMemory[EX_DM.alu_result_out + 1] = EX_DM.write_data_out << 8 >> 24;
			Memory::DMemory[EX_DM.alu_result_out + 2] = EX_DM.write_data_out << 16 >> 24;
			Memory::DMemory[EX_DM.alu_result_out + 3] = EX_DM.write_data_out << 24 >> 24;
		} else if (EX_DM.mem_op_out == MEM_HALF) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 1 >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (EX_DM.alu_result_out % 2) {
				Terminal::dataMisaaligned = 1;
			}
			if (Terminal::memoryOverflow || Terminal::dataMisaaligned) return;
			Memory::DMemory[EX_DM.alu_result_out] = EX_DM.write_data_out << 16 >> 24;
			Memory::DMemory[EX_DM.alu_result_out + 1] = EX_DM.write_data_out << 24 >> 24;
		} else if (EX_DM.mem_op_out == MEM_BYTE) {
			if (EX_DM.alu_result_out >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (Terminal::memoryOverflow) return;
			Memory::DMemory[EX_DM.alu_result_out] = EX_DM.write_data_out << 24 >> 24;
		} else printf("Error at DM.c.\n");
	}

	if (EX_DM.mem_read_out == 1) {
		if (EX_DM.mem_op_out == MEM_WORD) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 3 >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (EX_DM.alu_result_out % 4) {
				Terminal::dataMisaaligned = 1;
			}
			if (Terminal::memoryOverflow || Terminal::dataMisaaligned) return;
			for (int i = 0; i < 4; i++)
				DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) Memory::DMemory[EX_DM.alu_result_out + i];
		} else if (EX_DM.mem_op_out == MEM_HALF) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 1 >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (EX_DM.alu_result_out % 2) {
				Terminal::dataMisaaligned = 1;
			}
			if (Terminal::memoryOverflow || Terminal::dataMisaaligned) return;
			DM_WB.read_data_in = Memory::DMemory[EX_DM.alu_result_out];
			DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) Memory::DMemory[EX_DM.alu_result_out + 1];
		} else if (EX_DM.mem_op_out == MEM_HALF_UNSIGN) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 1 >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (EX_DM.alu_result_out % 2) {
				Terminal::dataMisaaligned = 1;
			}
			if (Terminal::memoryOverflow || Terminal::dataMisaaligned) return;
			DM_WB.read_data_in = (unsigned char) Memory::DMemory[EX_DM.alu_result_out];
			DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) Memory::DMemory[EX_DM.alu_result_out + 1];
		} else if (EX_DM.mem_op_out == MEM_BYTE) {
			if (EX_DM.alu_result_out >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (Terminal::memoryOverflow) return;
			DM_WB.read_data_in = Memory::DMemory[EX_DM.alu_result_out];
		} else if (EX_DM.mem_op_out == MEM_BYTE_UNSIGN) {
			if (EX_DM.alu_result_out >= 1024) {
				Terminal::memoryOverflow = 1;
			}
			if (Terminal::memoryOverflow) return;
			DM_WB.read_data_in = (unsigned char) Memory::DMemory[EX_DM.alu_result_out];
		} else printf("Error at DM.c.\n");
	}

}

void WB() {
	if (DM_WB.reg_write_out == 1) {
		if (DM_WB.mem_to_reg_out == 1) Register::reg[DM_WB.reg_to_write_out] = DM_WB.read_data_out;
		else Register::reg[DM_WB.reg_to_write_out] = DM_WB.alu_result_out;

		if (DM_WB.reg_to_write_out == 0) {
			if (DM_WB.opcode_out == R && DM_WB.funct_out == SLL) {
				if (!(DM_WB.rt_out == 0 && DM_WB.shamt_out == 0)) {
					Terminal::write2Zero = 1;
					Register::reg[DM_WB.reg_to_write_out] = 0;
				}
			} else {
				Terminal::write2Zero = 1;
				Register::reg[DM_WB.reg_to_write_out] = 0;
			}
		}
	}

	if (DM_WB.opcode_out == HALT) Terminal::WB_halt = 1;
	else Terminal::WB_halt = 0;
}

void move() {
	//DM2WB
	DM_WB.opcode_out = DM_WB.opcode_in;
	DM_WB.rd_out = DM_WB.rd_in;
	DM_WB.rs_out = DM_WB.rs_in;
	DM_WB.rt_out = DM_WB.rt_in;
	DM_WB.funct_out = DM_WB.funct_in;
	DM_WB.shamt_out = DM_WB.shamt_in;

	DM_WB.reg_write_out = DM_WB.reg_write_in;
	DM_WB.mem_to_reg_out = DM_WB.mem_to_reg_in;
	DM_WB.alu_result_out = DM_WB.alu_result_in;
	DM_WB.reg_to_write_out = DM_WB.reg_to_write_in;

	DM_WB.read_data_out = DM_WB.read_data_in;

	//EX2DM
	EX_DM.mem_read_out = EX_DM.mem_read_in;
	EX_DM.mem_write_out = EX_DM.mem_write_in;
	EX_DM.mem_op_out = EX_DM.mem_op_in;
	EX_DM.rd_out = EX_DM.rd_in;
	EX_DM.rs_out = EX_DM.rs_in;
	EX_DM.rt_out = EX_DM.rt_in;
	EX_DM.shamt_out = EX_DM.shamt_in;
	EX_DM.funct_out = EX_DM.funct_in;

	EX_DM.reg_write_out = EX_DM.reg_write_in;
	EX_DM.mem_to_reg_out = EX_DM.mem_to_reg_in;
	EX_DM.write_data_out = EX_DM.write_data_in;

	EX_DM.opcode_out = EX_DM.opcode_in;
	EX_DM.$rs_out = EX_DM.$rs_in;
	EX_DM.$rt_out = EX_DM.$rt_in;
	EX_DM.reg_to_write_out = EX_DM.reg_to_write_in;
	EX_DM.alu_result_out = EX_DM.alu_result_in;

	//ID2EX
	if (STALL) {
		ID_EX.pc_plus_four_out = 0;
		ID_EX.opcode_out = 0;
		ID_EX.funct_out = 0;
		ID_EX.shamt_out = 0;
		ID_EX.rt_out = 0;
		ID_EX.rd_out = 0;
		ID_EX.rs_out = 0;

		ID_EX.reg_dst_out = 0;
		ID_EX.alu_src_out = 0;

		ID_EX.mem_read_out = 0;
		ID_EX.mem_write_out = 0;
		ID_EX.mem_op_out = 0;

		ID_EX.reg_write_out = 0;
		ID_EX.mem_to_reg_out = 0;

		ID_EX.$rs_out = 0;
		ID_EX.$rt_out = 0;
		ID_EX.extended_imme_out = 0;

		ID_EX.pc_src_out = 0;
		ID_EX.pc_out = 0;
		ID_EX.reg_to_write_out = 0;
	} else {
		ID_EX.pc_plus_four_out = ID_EX.pc_plus_four_in;
		ID_EX.opcode_out = ID_EX.opcode_in;
		ID_EX.funct_out = ID_EX.funct_in;
		ID_EX.shamt_out = ID_EX.shamt_in;
		ID_EX.rt_out = ID_EX.rt_in;
		ID_EX.rd_out = ID_EX.rd_in;
		ID_EX.rs_out = ID_EX.rs_in;

		ID_EX.reg_dst_out = ID_EX.reg_dst_in;
		ID_EX.alu_src_out = ID_EX.alu_src_in;

		ID_EX.mem_read_out = ID_EX.mem_read_in;
		ID_EX.mem_write_out = ID_EX.mem_write_in;
		ID_EX.mem_op_out = ID_EX.mem_op_in;

		ID_EX.reg_write_out = ID_EX.reg_write_in;
		ID_EX.mem_to_reg_out = ID_EX.mem_to_reg_in;

		ID_EX.$rs_out = ID_EX.$rs_in;
		ID_EX.$rt_out = ID_EX.$rt_in;
		ID_EX.extended_imme_out = ID_EX.extended_imme_in;

		ID_EX.pc_src_out = ID_EX.pc_src_in;
		ID_EX.pc_out = ID_EX.pc_in;
		ID_EX.reg_to_write_out = ID_EX.reg_to_write_in;
	}

	//IF2ID
	if (STALL) return;
	IF_ID.pc_plus_four_out = IF_ID.pc_plus_four_in;
	IF_ID.ins_reg_out = IF_ID.ins_reg_in;

	IF_ID.opcode_out = IF_ID.opcode_in;
	IF_ID.funct_out = IF_ID.funct_in;
	IF_ID.rs_out = IF_ID.rs_in;
	IF_ID.rt_out = IF_ID.rt_in;

}

void update() {
	ID_EX.pc_plus_four_in = IF_ID.pc_plus_four_out;
	ID_EX.opcode_in = IF_ID.opcode_out;
	ID_EX.funct_in = IF_ID.funct_out;
	ID_EX.shamt_in = IF_ID.ins_reg_out << 21 >> 27;
	ID_EX.rs_in = IF_ID.rs_out;
	ID_EX.rt_in = IF_ID.rt_out;
	ID_EX.rd_in = IF_ID.ins_reg_out << 16 >> 27;
	/********************************************************/
	EX_DM.opcode_in = ID_EX.opcode_out;
	EX_DM.rd_in = ID_EX.rd_out;
	EX_DM.rt_in = ID_EX.rt_out;
	EX_DM.rs_in = ID_EX.rs_out;
	EX_DM.funct_in = ID_EX.funct_out;
	EX_DM.shamt_in = ID_EX.shamt_out;

	EX_DM.mem_read_in = ID_EX.mem_read_out;
	EX_DM.mem_write_in = ID_EX.mem_write_out;
	EX_DM.mem_op_in = ID_EX.mem_op_out;

	EX_DM.reg_write_in = ID_EX.reg_write_out;
	EX_DM.mem_to_reg_in = ID_EX.mem_to_reg_out;

	EX_DM.$rs_in = ID_EX.$rs_out;
	EX_DM.$rt_in = ID_EX.$rt_out;
	EX_DM.reg_to_write_in = ID_EX.reg_to_write_out;
	/**********************************************************/
	DM_WB.opcode_in = EX_DM.opcode_out;
	DM_WB.rd_in = EX_DM.rd_out;
	DM_WB.rt_in = EX_DM.rt_out;
	DM_WB.rs_in = EX_DM.rs_out;
	DM_WB.shamt_in = EX_DM.shamt_out;
	DM_WB.funct_in = EX_DM.funct_out;

	DM_WB.reg_write_in = EX_DM.reg_write_out;
	DM_WB.mem_to_reg_in = EX_DM.mem_to_reg_out;

	DM_WB.alu_result_in = EX_DM.alu_result_out;
	DM_WB.reg_to_write_in = EX_DM.reg_to_write_out;
}
