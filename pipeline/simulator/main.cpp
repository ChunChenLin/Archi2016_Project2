#include <stdio.h>
#include <stdlib.h>

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
bool stall;

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
    Register::PC = tmp;

    for(int i=4; i<8; i++) {
        num = (num<<8) + (unsigned char)iimageBuffer[i];
    }

    index = Register::PC;
    for(int i=8; i<8+4*num; i++) {
        Memory::IMemory[index++] = iimageBuffer[i];
    }
}

void errorDump() {
    if (Terminal::write2Zero) {
        Terminal::write2Zero = false;
        fprintf(error_dump, "In cycle %d: Write $0 Error\n", Register::cycle);
        printf("In cycle %d: Write $0 Error\n", Register::cycle);
    }
    if (Terminal::numberOverflow) {
        Terminal::numberOverflow = false;
        fprintf(error_dump, "In cycle %d: Number Overflow\n", Register::cycle);
        printf("In cycle %d: Number Overflow\n", Register::cycle);
    }
    if (Terminal::memoryOverflow) {
        Terminal::memoryOverflow = false;
        fprintf(error_dump, "In cycle %d: Address Overflow\n", Register::cycle);
        printf("In cycle %d: Address Overflow\n", Register::cycle);
    }
    if (Terminal::dataMisaaligned) {
        Terminal::dataMisaaligned = false;
        fprintf(error_dump, "In cycle %d: Misalignment Error\n", Register::cycle);
        printf("In cycle %d: Misalignment Error\n", Register::cycle);
    }
}

void printIns(unsigned op, unsigned rt, unsigned rd, unsigned func, unsigned shamt) {
    //printf("In printIns -- op=%d, rt=%d, rd=%d, func=%d shamt=%d\n", op, rt, rd, func, shamt);
    switch (op) {
        case R:
            switch (func) {
                case ADD:
                    fprintf(snapshot, "ADD");
                    printf("ADD");
                    break;
                case ADDU:
                    fprintf(snapshot, "ADDU");
                    printf("ADDU");
                    break;
                case SUB:
                    fprintf(snapshot, "SUB");
                    printf("SUB");
                    break;
                case AND:
                    fprintf(snapshot, "AND");
                    printf("AND");
                    break;
                case OR:
                    fprintf(snapshot, "OR");
                    printf("OR");
                    break;
                case XOR:
                    fprintf(snapshot, "XOR");
                    printf("XOR");
                    break;
                case NOR:
                    fprintf(snapshot, "NOR");
                    printf("NOR");
                    break;
                case NAND:
                    fprintf(snapshot, "NAND");
                    printf("NAND");
                    break;
                case SLT:
                    fprintf(snapshot, "SLT");
                    printf("SLT");
                    break;
                case SLL:
                    if (rt == 0 && rd == 0 && shamt == 0) {
                        fprintf(snapshot, "NOP");
                        printf("NOP");
                    }
                    else {
                        fprintf(snapshot, "SLL");
                        printf("SLL");
                    }
                    break;
                case SRL:
                    fprintf(snapshot, "SRL");
                    printf("SRL");
                    break;
                case SRA:
                    fprintf(snapshot, "SRA");
                    printf("SRA");
                    break;
                case JR:
                    fprintf(snapshot, "JR");
                    printf("JR");
                    break;
                default:
                    fprintf(snapshot, "ERROR!!");
                    printf("ERROR!!");
                    break;
            }
            break;
        case ADDI:
            fprintf(snapshot, "ADDI");
            printf("ADDI");
            break;
        case ADDIU:
            fprintf(snapshot, "ADDIU");
            printf("ADDIU");
            break;
        case LW:
            fprintf(snapshot, "LW");
            printf("LW");
            break;
        case LH:
            fprintf(snapshot, "LH");
            printf("LH");
            break;
        case LHU:
            fprintf(snapshot, "LHU");
            printf("LHU");
            break;
        case LB:
            fprintf(snapshot, "LB");
            printf("LB");
            break;
        case LBU:
            fprintf(snapshot, "LBU");
            printf("LBU");
            break;
        case SW:
            fprintf(snapshot, "SW");
            printf("SW");
            break;
        case SH:
            fprintf(snapshot, "SH");
            printf("SH");
            break;
        case SB:
            fprintf(snapshot, "SB");
            printf("SB");
            break;
        case LUI:
            fprintf(snapshot, "LUI");
            printf("SB");
            break;
        case ANDI:
            fprintf(snapshot, "ANDI");
            printf("ANDI");
            break;
        case ORI:
            fprintf(snapshot, "ORI");
            printf("ORI");
            break;
        case NORI:
            fprintf(snapshot, "NORI");
            printf("NORI");
            break;
        case SLTI:
            fprintf(snapshot, "SLTI");
            printf("SLTI");
            break;
        case BEQ:
            fprintf(snapshot, "BEQ");
            printf("BEQ");
            break;
        case BNE:
            fprintf(snapshot, "BNE");
            printf("BNE");
            break;
        case BGTZ:
            fprintf(snapshot, "BGTZ");
            printf("BGTZ");
            break;
        case J:
            fprintf(snapshot, "J");
            printf("J");
            break;
        case JAL:
            fprintf(snapshot, "JAL");
            printf("JAL");
            break;
        case HALT:
            fprintf(snapshot, "HALT");
            printf("HALT");
            break;
        default:
            fprintf(snapshot, "ERROR!!");
            printf("ERROR!!");
            break;
    }
}

void snapShotForReg() {
    fprintf(snapshot, "cycle %d\n", Register::cycle++);
    //printf("Register::cycle %d\n", Register::cycle++);
    for(int i=0; i<32; i++) {
        fprintf(snapshot, "$%02d: 0x", i);
        fprintf(snapshot, "%08X\n", Register::reg[i]);
        printf("$%02d: 0x", i);
        printf("%08X\n", Register::reg[i]);
    }

    if (ID_EX.pc_src_out) {
        fprintf(snapshot, "PC: 0x%08X\n", ID_EX.pc_out);
        printf("PC: 0x%08X\n", ID_EX.pc_out);
        fprintf(snapshot, "IF: 0x");
        printf("IF: 0x");
        for (int i = 0; i < 4; i++) {
            fprintf(snapshot, "%02X", Memory::IMemory[ID_EX.pc_out + i] & 0xff);
            printf("%02X", Memory::IMemory[ID_EX.pc_out + i] & 0xff);
        }
    } else {
        fprintf(snapshot, "PC: 0x%08X\n", Register::PC);
        printf("PC: 0x%08X\n", Register::PC);
        fprintf(snapshot, "IF: 0x");
        printf("IF: 0x");
        for (int i = 0; i < 4; i++) {
            fprintf(snapshot, "%02X", Memory::IMemory[Register::PC + i] & 0xff);
            printf("%02X", Memory::IMemory[Register::PC + i] & 0xff);
        }
    }
}

void snapShotForStages() {
    if (stall) {
        fprintf(snapshot, " to_be_stalled");
        printf(" to_be_stalled");
    }
    else if (ID_EX.pc_src_in == 1) {
        fprintf(snapshot, " to_be_flushed");
        printf(" to_be_flushed");
    }
    fprintf(snapshot, "\n");
    printf("\n");

    fprintf(snapshot, "ID: ");
    printf("ID: ");
    if (ID_EX.pc_src_out) {
        fprintf(snapshot, "NOP");
        printf("NOP");
    }
    else printIns(IF_ID.inst_out >> 26, IF_ID.inst_out << 11 >> 27, IF_ID.inst_out << 16 >> 27, IF_ID.inst_out << 26 >> 26, IF_ID.inst_out << 21 >> 27);
    if (stall) {
        fprintf(snapshot, " to_be_stalled");
        printf(" to_be_stalled");
    } else {
        if (Forward::rs2ID) {
            fprintf(snapshot, " fwd_EX-DM_rs_$%u", IF_ID.rs_out);
            printf(" fwd_EX-DM_rs_$%u", IF_ID.rs_out);
        }
        if (Forward::rt2ID) {
            fprintf(snapshot, " fwd_EX-DM_rt_$%u", IF_ID.rt_out);
            printf(" fwd_EX-DM_rt_$%u", IF_ID.rt_out);
        }
    }
    fprintf(snapshot, "\n");
    printf("\n");

    fprintf(snapshot, "EX: ");
    printf("EX: ");
    printIns(ID_EX.op_out, ID_EX.rt_out, ID_EX.rd_out, ID_EX.func_out, ID_EX.shamt_out);
    if (Forward::rs2EX) {
        fprintf(snapshot, " fwd_EX-DM_rs_$%u", ID_EX.rs_out);
        printf(" fwd_EX-DM_rs_$%u", ID_EX.rs_out);
    }
    if (Forward::rt2EX) {
        fprintf(snapshot, " fwd_EX-DM_rt_$%u", ID_EX.rt_out);
        printf(" fwd_EX-DM_rt_$%u", ID_EX.rt_out);
    }
    fprintf(snapshot, "\n");
    printf("\n");

    fprintf(snapshot, "DM: ");
    printf("DM: ");
    printIns(EX_DM.op_out, EX_DM.rt_out, EX_DM.rd_out, EX_DM.func_out, EX_DM.shamt_out);
    fprintf(snapshot, "\n");
    printf("\n");

    fprintf(snapshot, "WB: ");
    printf("WB: ");
    printIns(DM_WB.op_out, DM_WB.rt_out, DM_WB.rd_out, DM_WB.func_out, DM_WB.shamt_out);
    fprintf(snapshot, "\n\n\n");
    printf("\n\n\n");
}

int main() {
    Open();
    DImg();
    IImg();

    Terminal::WB_halt = false;
	Terminal::write2Zero = false;
    Terminal::numberOverflow = false;
    Terminal::memoryOverflow = false;
	Terminal::dataMisaaligned = false;

    // IF instr-> detect stall -> snapshot -> assign reg memory ->  move buffer
    while(Terminal::WB_halt==false) {
        printf("---------------------------------------------------------------\n\n");

        checkForwarding();
        checkStall();
        snapShotForReg();

        //printf("calling WB().................\n");
        WB();
        //printf("calling DM().................\n");
        DM();
        //printf("calling EX().................\n");
        EX();
        //printf("calling ID().................\n");
        ID();
        //printf("calling IF().................\n");
        IF(); //fetch instruction

        snapShotForStages();

        DM2WB();
        EX2DM();
        ID2EX();
        IF2ID();

        errorDump();

        printf("---------------------------------------------------------------\n\n");
    }

    return 0;
}
