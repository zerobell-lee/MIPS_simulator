#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mips.h"

void alu(int rd, int rs, int rt, int fn) {
	switch (fn)
	{
	case 8:
		jr(rs); //jump를 하는 명령어는 PC를 직접 변경하므로 advance_PC(4)가 아래에 나타나지 않습니다.
		return;
	case 32:
		add(rd, rs, rt);
		advance_PC(4);
		return;
	case 34:
		sub(rd, rs, rt);
		advance_PC(4);
		return;
	case 36:
		and(rd, rs, rt);
		advance_PC(4);
		return;
	case 37:
		or(rd, rs, rt);
		advance_PC(4);
		return;
	case 38:
		xor(rd, rs, rt);
		advance_PC(4);
		return;
	case 39:
		nor(rd, rs, rt);
		advance_PC(4);
		return;
	case 42:
		slt(rd, rs, rt);
		advance_PC(4);
		return;
	default:
		printf("Undefined Funtion!"); //fn이 듣도 보도 못한 값이라면 오류 메시지를 출력합니다.
		return;
	}
}

void and(int rd, int rs, int rt) {
	R[rd] = R[rs] & R[rt];
}

void or (int rd, int rs, int rt) {
	R[rd] = R[rs] | R[rt];
}

void xor(int rd, int rs, int rt) {
	R[rd] = R[rs] ^ R[rt];
}

void nor(int rd, int rs, int rt) {
	R[rd] = ~(R[rs] | R[rt]);
}

void add(int rd, int rs, int rt) {
	R[rd] = R[rs] + R[rt];
}
void sub(int rd, int rs, int rt) {
	R[rd] = R[rs] - R[rt];
}
void slt(int rd, int rs, int rt) {
	if (R[rs] < R[rt]) R[rd] = 1;
	else R[rd] = 0;
}

int syscall() {
	switch (R[2]) {
	case 10:
		return 10;
	default:
		printf("Undefined System Call Code!!\n");
		return 0;
	}
	return;
}

void jr(int rs) {
	PC = R[rs];
}



