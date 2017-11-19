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
	switch (op) //op code�� �Ǻ��Ͽ� � ����� �������� �����մϴ�.
	{
	case 1:
		bltz(rs, offset); //branch �迭 �Լ����� PC ���� �����ϹǷ� advance_PC() �Լ��� �Ʒ��� ������ �ʽ��ϴ�.
		return;
	case 4:
		beq(rs, rt, offset);
		return;
	case 5:
		bne(rs, rt, offset);
		return;
	case 8:
		addi(rt, rs, offset); //������ �Լ� ���� ��, ���� ��ɾ ������ �� �ֵ��� PC�� 4�� �������ϴ�.
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
	R[31] = PC + 8; //�ٽ� ���ƿ� $ra�� ���� PC���� ���� �ڷ� �����˴ϴ�.
	PC = ((PC&0xf0000000)|addr);
}
void jmp(U addr) {
	PC = ((PC&0xf0000000)|addr);
}