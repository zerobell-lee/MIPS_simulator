#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mips.h"

void andi(int rt, int rs, int offset) {
	R[rt] = R[rs] & (0x0000ffff&offset);
}

void ori(int rt, int rs, int offset) {
	R[rt] = R[rs] | (0x0000ffff&offset);
}
void xori(int rt, int rs, int offset) {
	R[rt] = R[rs] ^ (0x0000ffff&offset);
}
void bltz(int rs, int offset) {
	if ((0x80000000&R[rs])==0x80000000) {
		advance_PC(offset<<2);
	}
	else advance_PC(4);
}

void beq(int rs, int rt, int offset) {
	if (R[rs] == R[rt]) {
		advance_PC(offset<<2);
	}
	else { 
		advance_PC(4); 
	}
}

void bne(int rs, int rt, int offset) {
	if (R[rs] != R[rt]) {
		advance_PC(offset<<2);
	}
	else advance_PC(4);
}

void addi(int rt, int rs, int offset) {
	R[rt] = R[rs] + offset;
}
void slti(int rt, int rs, int offset) {
	if ((int)(R[rs]) < offset) R[rt] = 1;
	else R[rt] = 0;
}
void lw(int rt, int rs, int offset) {
	U addr = R[rs] + offset;
	int arry = cvt_arry(addr);
	R[rt] = M[arry];
}
void sw(int rt, int rs, int offset) {
	U addr = R[rs] + offset;
	int arry = cvt_arry(addr);
	M[arry] = R[rt];
}
void lui(int rt, int offset) {
	offset = offset << 16;
	R[rt] = 0;
	R[rt] = (R[rt] | offset);
}

/////

void i_type(int op, int rs, int rt, int offset) {
	switch (op) //op code를 판별하여 어떤 명령을 실행할지 결정합니다.
	{
	case 1:
		bltz(rs, offset); //branch 계열 함수들은 PC 값을 변경하므로 advance_PC() 함수가 아래에 나오지 않습니다.
		return;
	case 4:
		beq(rs, rt, offset);
		return;
	case 5:
		bne(rs, rt, offset);
		return;
	case 8:
		addi(rt, rs, offset); //보통은 함수 실행 후, 다음 명령어를 가져올 수 있도록 PC에 4가 더해집니다.
		advance_PC(4);
		return;
	case 10:
		slti(rt, rs, offset);
		advance_PC(4);
		return;
	case 12:
		andi(rt, rs, offset);
		advance_PC(4);
		return;
	case 13:
		ori(rt, rs, offset);
		advance_PC(4);
		return;
	case 14:
		xori(rt, rs, offset);
		advance_PC(4);
		return;
	case 15:
		lui(rt, offset);
		advance_PC(4);
		return;
	case 35:
		lw(rt, rs, offset);
		advance_PC(4);
		return;
	case 43:
		sw(rt, rs, offset);
		advance_PC(4);
		return;
	default:
		printf("Undefined Opcode!!(%d)", op);
		advance_PC(4);
		return;
	}
}

void jal(U addr) {
	R[31] = PC + 8; //다시 돌아올 $ra가 현재 PC에서 한줄 뒤로 설정됩니다.
	PC = ((PC&0xf0000000)|addr);
}
void jmp(U addr) {
	PC = ((PC&0xf0000000)|addr);
}