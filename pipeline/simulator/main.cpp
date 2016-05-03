#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "global.h"
#include "define.h"
#include "check.h"
#include "stages.h"

FILE *iimage, *dimage, *error_dump, *snapshot;
unsigned iimageLen, dimageLen;
char *iimageBuffer, *dimageBuffer;

Buffer IF_ID;
Buffer ID_EX;
Buffer EX_DM;
Buffer DM_WB;
Buffer WB_END;
bool STALL;

//map<int, string> ins;
//map<int, string> rIns;
char* ins[65];
char* rIns[45];

void Open() {
    iimage = fopen("iimage.bin", "rb");
    dimage = fopen("dimage.bin", "rb");
    error_dump = fopen("error_dump.rpt", "wb");
    snapshot = fopen("snapshot.rpt", "wb");

    fseek(iimage, 0, SEEK_END);
    fseek(dimage, 0, SEEK_END);

    iimageLen = (unsigned)ftell(iimage);
    dimageLen = (unsigned)ftell(dimage);

    // move to the beginning of the file
    rewind(iimage);
    rewind(dimage);

    // allocate memory
    iimageBuffer = new char[iimageLen];
    dimageBuffer = new char[dimageLen];

    fread(iimageBuffer, 1, iimageLen, iimage);
    fread(dimageBuffer, 1, dimageLen, dimage);

    fclose(iimage);
    fclose(dimage);
}

void DImg() {
    unsigned tmp=0, num=0, index=0;

    for(int i=0; i<4; i++) {
        tmp = (tmp<<8) + (unsigned char)dimageBuffer[i];
    }
    Register::reg[29] = tmp; //$SP

    for(int i=4; i<8; i++) {
        num = (num<<8) + (unsigned char)dimageBuffer[i];
    }

    for(int i=8; i<8+4*num; i++) {
        Memory::DMemory[index++] = dimageBuffer[i];
    }
}

void IImg() {
    unsigned tmp=0, num=0, index=0;

    for(int i=0; i<4; i++) {
        tmp = (tmp<<8) + (unsigned char)iimageBuffer[i];
    }
    IF_ID.pc_plus_four_out = tmp;
    //printf("---IF_ID.pc_plus_four_out= 0x%08X\n\n",IF_ID.pc_plus_four_out);
    Register::PC = tmp;

    for(int i=4; i<8; i++) {
        num = (num<<8) + (unsigned char)iimageBuffer[i];
    }

    index = Register::PC;
    for(int i=8; i<8+4*num; i++) {
        Memory::IMemory[index++] = iimageBuffer[i];
    }
}

void setInstructions(){ // fast transfer
    // R
    rIns[ADD] = "ADD";
    rIns[ADDU] = "ADDU";
    rIns[SUB] = "SUB";
    rIns[AND] = "AND";
    rIns[OR] = "OR";
    rIns[XOR] = "XOR";
    rIns[NOR] = "NOR";
    rIns[NAND] = "NAND";
    rIns[SLT] = "SLT";
    rIns[SLL] = "SLL";
    rIns[SRL] = "SRL";
    rIns[SRA] = "SRA";
    rIns[JR] = "JR";
    // I
    ins[ADDI] = "ADDI";
    ins[ADDIU] = "ADDIU";
    ins[LW] = "LW";
    ins[LH] = "LH";
    ins[LHU] = "LHU";
    ins[LB] = "LB";
    ins[LBU] = "LBU";
    ins[SW] = "SW";
    ins[SH] = "SH";
    ins[SB] = "SB";
    ins[LUI] = "LUI";
    ins[ANDI] = "ANDI";
    ins[ORI] = "ORI";
    ins[NORI] = "NORI";
    ins[SLTI] = "SLTI";
    ins[BEQ] = "BEQ";
    ins[BNE] = "BNE";
    ins[BGTZ] = "BGTZ";
    // J
    ins[J] = "J";
    ins[JAL] = "JAL";
    // S
    ins[HALT] = "HALT";
}

void errorDump() {
    if (Terminal::write2Zero) {
        fprintf(error_dump, "In cycle %d: Write $0 Error\n", Register::cycle);
        //printf("In cycle %d: Write $0 Error\n", Register::cycle);
    }
    if (Terminal::memoryOverflow) {
        fprintf(error_dump, "In cycle %d: Address Overflow\n", Register::cycle);
        //printf("In cycle %d: Address Overflow\n", Register::cycle);
    }
    if (Terminal::dataMisaaligned) {
        fprintf(error_dump, "In cycle %d: Misalignment Error\n", Register::cycle);
        //printf("In cycle %d: Misalignment Error\n", Register::cycle);
    }
    if (Terminal::numberOverflow) {
        fprintf(error_dump, "In cycle %d: Number Overflow\n", Register::cycle);
        //printf("In cycle %d: Number Overflow\n", Register::cycle);
    }   
}

void snapShotForReg() {
    fprintf(snapshot, "cycle %d\n", Register::cycle++);
    //printf("Register::cycle %d\n", Register::cycle++);
    for(int i=0; i<32; i++) {
        fprintf(snapshot, "$%02d: 0x", i);
        fprintf(snapshot, "%08X\n", Register::reg[i]);
        //printf("$%02d: 0x", i);
        //printf("%08X\n", Register::reg[i]);
    }
    
    if (ID_EX.pc_src_out) {
        //printf("PC_SRC_OUT...\n");
        fprintf(snapshot, "PC: 0x%08X\n", ID_EX.pc_out);
        //printf("PC: 0x%08X\n", ID_EX.pc_out);
        fprintf(snapshot, "IF: 0x");
        //printf("IF: 0x");
        for (int i = 0; i < 4; i++) {
            fprintf(snapshot, "%02X", Memory::IMemory[ID_EX.pc_out + i] & 0xff);
            //printf("%02X", Memory::IMemory[ID_EX.pc_out + i] & 0xff);
        }
    } else {
        //printf("else ...\n");
        fprintf(snapshot, "PC: 0x%08X\n", Register::PC);
        //printf("PC: 0x%08X\n", Register::PC);
        fprintf(snapshot, "IF: 0x");
        //printf("IF: 0x");
        for (int i = 0; i < 4; i++) {
            fprintf(snapshot, "%02X", Memory::IMemory[Register::PC + i] & 0xff);
            //printf("%02X", Memory::IMemory[Register::PC + i] & 0xff);
        }
    }
    
}

void snapShotForStages() {
    if (STALL) {
        fprintf(snapshot, " to_be_stalled");
    } else if (ID_EX.pc_src_in == 1) {
        fprintf(snapshot, " to_be_flushed");
    }
    fprintf(snapshot, "\n");
    bool isNOP = false;

    isNOP = (ID_EX.pc_src_out)||(IF_ID.opcode_out==R && IF_ID.funct_out==SLL && IF_ID.rt_out==0 && IF_ID.rd_out==0 && IF_ID.shamt_out==0);
    fprintf(snapshot, "ID: %s%s", (isNOP) ? "NOP" : (IF_ID.opcode_out == R) ? rIns[IF_ID.funct_out] : ins[IF_ID.opcode_out], (STALL) ? " to_be_stalled" : "");
    //if(!STALL && (Forward::rs2ID||Forward::rt2ID))fprintf(snapshot, " fwd_EX-DM_%s_$%d", (Forward::rs2ID) ? "rs" : "rt", (Forward::rs2ID) ? IF_ID.rs_out : IF_ID.rt_out);
    if (!STALL&&!ID_EX.pc_src_out && Forward::rs2ID) fprintf(snapshot, " fwd_EX-DM_rs_$%u", IF_ID.rs_out);
    if (!STALL&&!ID_EX.pc_src_out && Forward::rt2ID) fprintf(snapshot, " fwd_EX-DM_rt_$%u", IF_ID.rt_out);
    fprintf(snapshot, "\n");

    isNOP = ID_EX.opcode_out==R && ID_EX.funct_out==SLL && ID_EX.rt_out==0 && ID_EX.rd_out==0 && ID_EX.shamt_out==0;
    fprintf(snapshot, "EX: %s", (isNOP) ? "NOP" : (ID_EX.opcode_out == R) ? rIns[ID_EX.funct_out] : ins[ID_EX.opcode_out]);
    //if(!STALL && (Forward::rs2EX||Forward::rt2EX))fprintf(snapshot, " fwd_EX-DM_%s_$%d", (Forward::rs2EX) ? "rs" : "rt", (Forward::rs2EX) ? ID_EX.rs_out : ID_EX.rt_out);
    if (Forward::rs2EX) fprintf(snapshot, " fwd_EX-DM_rs_$%u", ID_EX.rs_out);
    if (Forward::rt2EX) fprintf(snapshot, " fwd_EX-DM_rt_$%u", ID_EX.rt_out);
    fprintf(snapshot, "\n");

    isNOP = EX_DM.opcode_out==R && EX_DM.funct_out==SLL && EX_DM.rt_out==0 && EX_DM.rd_out==0 && EX_DM.shamt_out==0;
    fprintf(snapshot, "DM: %s\n", (isNOP) ? "NOP" : (EX_DM.opcode_out == R) ? rIns[EX_DM.funct_out] : ins[EX_DM.opcode_out]);
    
    isNOP = DM_WB.opcode_out==R && DM_WB.funct_out==SLL && DM_WB.rt_out==0 && DM_WB.rd_out==0 && DM_WB.shamt_out==0;
    fprintf(snapshot, "WB: %s\n", (isNOP) ? "NOP" : (DM_WB.opcode_out == R) ? rIns[DM_WB.funct_out] : ins[DM_WB.opcode_out]);
    
    fprintf(snapshot, "\n\n");
} 

void initialize() {
    memset(&IF_ID, 0, sizeof(IF_ID));
    memset(&ID_EX, 0, sizeof(ID_EX));
    memset(&EX_DM, 0, sizeof(EX_DM));
    memset(&DM_WB, 0, sizeof(DM_WB));
}

/*bool detectHalt() {
    if ((Terminal::IF_HALT && Terminal::ID_HALT && Terminal::EX_HALT && Terminal::DM_HALT && Terminal::WB_HALT) || (Terminal::memoryOverflow || Terminal::dataMisaaligned)) return true;
    else return false;
}*/

int main() {
    initialize();

    Open();
    DImg();
    IImg();

    //printf("---IF_ID.pc_plus_four_out= 0x%08X\n\n",IF_ID.pc_plus_four_out);
    Terminal::IF_HALT = false;
    Terminal::ID_HALT = false;
    Terminal::EX_HALT = false;
    Terminal::DM_HALT = false;
    Terminal::WB_HALT = false;
	Terminal::write2Zero = false;
    Terminal::numberOverflow = false;
    Terminal::memoryOverflow = false;
	Terminal::dataMisaaligned = false;

    
    setInstructions();

    // IF instr-> detect stall -> snapshot -> assign reg memory ->  move buffer
    while(!(Terminal::WB_HALT || Terminal::memoryOverflow || Terminal::dataMisaaligned)) {
        Terminal::write2Zero = false;
        Terminal::numberOverflow = false;
        
        checkStall();
        checkForwarding();
        
        snapShotForReg();
        
        update();

        WB();
        DM();
        EX();
        ID();
        IF();

        snapShotForStages();

        move();

        errorDump();
    }

    return 0;
}
