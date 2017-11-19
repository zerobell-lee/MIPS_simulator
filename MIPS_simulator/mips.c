#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char DorH(char str[]) {
	if ((str[0] == '0') && (str[1] == 'x')) return 'H';
	else return 'D';
} //���ڿ��� 16�������� 10�������� �Ǻ��ϴ� �Լ�. ���ڿ��� 0x�� �����ϸ� 'H'��, �׷��� ������ 'D'�� ��ȯ�մϴ�.

int atoi_s(char str[]) {
	int num;
	if (DorH(str) == 'H') {
		int i;
		for (i = strlen(str) - 1; i > 1; i--) {
			if ((str[i] >= 'A') && (str[i] <= 'F')) {
				num += ((str[i] - 'A' + 10) << (strlen(str) - 1 - i) * 4);
			}
			else if ((str[i] >= 'a') && (str[i] <= 'f')) {
				num += ((str[i] - 'a' + 10) << (strlen(str) - 1 - i) * 4);
			}
			else {
				num += ((str[i] - '0') << (strlen(str) - 1 - i) * 4);
			}
		}
		return num;
	}
	else {
		num = atoi(str);
		return num;
	}
} // Signed Int�� ���ڿ��� ���ڷ� ��ȯ. ���ڿ��� 16�������� 10���������� �Ǻ��Ͽ� �׿� �´� ��ȯ������ ��Ĩ�ϴ�.

U atoi_u(char str[]) {
	U num=0;
	if (DorH(str) == 'H') {
		int i;
		for (i = strlen(str) - 1; i > 1; i--) {
			if ((str[i] >= 'A') && (str[i] <= 'F')) {
				num += ((str[i] - 'A' + 10) << (strlen(str) - 1 - i) * 4);
			}
			else if ((str[i] >= 'a') && (str[i] <= 'f')) {
				num += ((str[i] - 'a' + 10) << (strlen(str) - 1 - i) * 4);
			}
			else {
				num += ((str[i] - '0') << (strlen(str) - 1 - i) * 4);
			}
		}
		return num;
	}
	else {
		int i;
		U j = 1;
		for (i = strlen(str) - 1; i >= 0; i--) {
			num += ((str[i] - '0')*j);
			j *= 10;
		}
		return num;
	}
} // Unsigned Int�� ���ڿ��� ���ڷ� ��ȯ. ���ڿ��� 16�������� 10���������� �Ǻ��Ͽ� �׿� �´� ��ȯ������ ��Ĩ�ϴ�.

int cvt_arry(U num) {
	if ((num % 4) != 0) {
		printf("exception occurred\n");
		return FALSE;
	}
	return (num / 4);
} // 4�� ����� 4�� �������ִ� �Լ�. ���� 4�� ����� �ƴ϶�� exception occurred��� �޽����� �Բ� FALSE�� ��ȯ�մϴ�.

U cvt_addr(int num) {
	U num1 = num;
	return num1<<2;
} // �ּҿ� ��Ʈ 00�� �ٿ��ִ� �Լ�. jump�� branch ��ɾ Ȱ���մϴ�.

void advance_PC(int offset) {
	PC += offset;
} // offset��ŭ�� PC�� �����մϴ�. ������ advance_PC(4)�� ���·� 4�� �����ݴϴ�.

//Inst ó�� ���� �Լ���
int getOp(unsigned int instr) {
	return (instr>>26);
}
int getRs(unsigned int instr) {
	return ((instr << 6) >> 27);
}
int getRt(unsigned int instr) {
	return ((instr << 11) >> 27);
}
int getRd(unsigned int instr) {
	return ((instr << 16) >> 27);
}
int getSh(unsigned int instr) {
	return ((instr << 21) >> 27);
}
int getFn(unsigned int instr) {
	return ((instr<<26)>>26);
}
int getOffset(unsigned int instr) {
	int offset = (instr << 16);
	offset = offset >> 16;
	return offset;
} // Offset�� signed int�� ��ȯ�˴ϴ�. sign-extention�� �Ͼ�ϴ�.
U getOffset_u(unsigned int instr) {
	return ((instr << 16) >> 16);
} // �� �Լ��� ����ϸ� Offset�� unsigned int�� ��ȯ�մϴ�. �� zero-extention ���·� ��ȯ�� �ϰ� �˴ϴ�.
U getAddr(unsigned int instr) {
	return ((instr << 6) >> 6);
}

//DataPath�� ���õ� �Լ�
void fetch_instr(U* instr) {
	*instr = M[cvt_arry(PC)];
} //PC�� 1/4�� ���� M �迭���� ã�Ƽ� instr�� �Է��մϴ�.

int operation(U instr) {

	int op = getOp(instr); // ���� instr�� op�� ����Ͽ� �Ǻ��մϴ�.
	if (op == 0) { // op�� 0�̶�� ALU�� ��ġ�� ��ɾ��� �ǹ��Դϴ�. �� ��ɾ�� ���� R-Type�Դϴ�.
		int rd = getRd(instr); //�׷��Ƿ� rd, rs, rt, fn�� ����� �ʿ䰡 �ֽ��ϴ�.
		int rs = getRs(instr);
		int rt = getRt(instr);
		int fn = getFn(instr);
		alu(rd, rs, rt, fn);  //�������� ALU() �Լ��� �˾Ƽ� ó���� ���Դϴ�.
		return TRUE;
	}
	else if (op == 2) { // j ��ɾ�� I-Type�� R-Type�� �ƴϹǷ� ���� ó���մϴ�.
		U addr = getAddr(instr);
		addr = cvt_addr(addr);
		jmp(addr);
		return TRUE;
	}
	else if (op == 3) { // jal�� ���������� ���� ó���߽��ϴ�.
		U addr = getAddr(instr);
		addr = cvt_addr(addr);
		jal(addr);
		return TRUE;
	}
	else { // ���� R-Type�� J-Type ��ɾ ���ϸ� ���� I-Type ��ɾ �����Ƿ� else ������ ó���ϵ��� �մϴ�.
		int rs = getRs(instr);
		int rt = getRt(instr);
		int offset = getOffset(instr);
		i_type(op, rs, rt, offset); //I-Type ��ɾ�� �˾Ƽ� ó���մϴ�.
		return TRUE;
	}
}


void datapath(int n) { //Step ��ɾ�� Go ��ɾ��� ���̸� �α� ���Ͽ� int n�� ���� �޾Ƽ� ó���ϵ��� �Ͽ����ϴ�.
	do {
		U instr;
		fetch_instr(&instr); //PC�κ��� instr�� �޾ƿɴϴ�.
		if (instr == 0) { //���� 32��Ʈ ��ΰ� 0�̶��, �� nop�̶�� ������ ó�� ������ �־��ݴϴ�.
			char ch;
			printf("No Instruction!! Wanna Continue?(y/n)");
			ch = getchar();
			if ((ch == 'y') || (ch == 'Y')) {
				advance_PC(4); //������ ���Ѵٸ� ���� �ٷ� �Ѿ�� �ǰ�, �ƴ� ���� datapath�� ����˴ϴ�.
				continue;
			}
			else {
				break;
			}
		}
		else if (instr == 0x0000000c) { 
			break; //syscall�� ������ �� datapath�� ����˴ϴ�.
		}
		else operation(instr); //nop�� syscall�� �ƴ϶��, operation �Լ��� ���Ͽ� instr�� �������ݴϴ�.
	} while (n); 
	/*
	���� int n�� 0�̾��ٸ� do { } �ȿ� �ִ� ��ɾ �� �ѹ��� �����ϰ� ���ᰡ �˴ϴ�.
	�׷��� n�� 0�� �ƴ� �ٸ� ���ڶ�� syscall�� ���� ������ ���� �ݺ��� �����ϰ� �˴ϴ�.
	*/
}


void load(char path[], int bin_r) { //�ϵ�κ��� binary file�̳� text file�� �ҷ����� �Լ��Դϴ�.
	char checker;
	char input[33] = { 0, };

	if (bin_r) {
		FILE *fp = fopen(path, "rb");
		U instr;
		int i = 0;
		int k = 0;
		if (fp == NULL) {
			printf("File Doesn't Exist!\n");
			exit(-1);
		}

		while (fread(&instr, sizeof(unsigned int), 1, fp) == 1) {
			M[k] = instr;
			k++;
		}
		fclose(fp);
		 
	}
	else {
		FILE* fp = fopen(path, "rt"); //binary file�̶�� rb�� �Ͽ��� ������, ���ͳ� ������ ó���� ���ؼ� rt ���� �о����ϴ�.
		if (fp == NULL) {
			printf("File Doesn't Exist!\n");
			exit(-1);
		}
		U instr;
		U digit;

		int i = 0;
		int j;
		int k = 0;

		checker = fgetc(fp); //���� ���� �����ͷκ��� �� ���ڸ� �о�ɴϴ�. �� �̻� �о�� ���ڰ� ���ٸ� -1�� ��ȯ�˴ϴ�.
		while (checker != -1) { //�о�� ���ڰ� ���� ��Ȳ�� �� ������ ����ؼ� ���� �ݺ����� �ݺ��մϴ�.
			if (checker == '\n') {
				checker = fgetc(fp); //���๮�ڸ� ������ ��쿣 �����ϰ� ���� ���ڸ� �н��ϴ�. ���� �ݺ����� �ٽ� �����մϴ�.
				continue;
			}
			input[i] = checker; //�о�� ���ڰ� ���๮�ڰ� �ƴϾ��ٸ� input �迭�� �ش� ���ڸ� �����մϴ�.
			if (i == 31) { //�迭 32���� ���� �� á�ٸ�, �̸� instr�� �ٲ�����մϴ�.
				instr = 0;
				for (j = 0; j < 32; j++) {
					if (input[j] == '0') digit = 0;
					else digit = 1;
					instr += digit << (31 - j); //32���� 0�� 1�� Unsigned Int ���·� ��ȯ��ŵ�ϴ�.
				}
				M[k] = instr; //�޸𸮸� ���ϴ� M �迭�� �ش� instr�� �����մϴ�.
				k++; //���� �޸𸮸� ���� �غ� �մϴ�.
				i = 0;

			}
			else {
				i++; //���� �迭�� 32���� ���� ���� �ʾҴٸ� �н��մϴ�.
			}
			checker = fgetc(fp); //�ٽ� ���� ���ڸ� �о���Դϴ�.
		}
		fclose(fp);
	}
}

void showmemory(char stok1[], char stok2[]) { //stok1���� stok2�� �ش��ϴ� ������ �޸𸮸� ǥ���մϴ�.
	U n1 = atoi_u(stok1);
	U n2 = atoi_u(stok2);

	int i = cvt_arry(n1);
	if (i == FALSE) return;
	while (i <= cvt_arry(n2)) {
		printf("[%08x] : 0x%08x\n", cvt_addr(i), M[i]); //stok�� 10������ 16������, 4�� ����̱⸸ �ϴٸ� ��� �����ϴ�.
		i++;
	}
}

void showregister(void) { //32���� �������͸� ���� ǥ���ϴ� �Լ��Դϴ�. PC�� ǥ���մϴ�.
	int i = 0;
	for (i = 0; i < 32; i++)
		printf("R[%d] : 0x%08x\n", i, R[i]);
	printf("PC : 0x%08x\n", PC);

}

void setregister(char stok1[], char stok2[]) { //Ư�� ���������� ���� �ٲپ��ִ� �Լ��Դϴ�.
	int n1 = atoi_s(stok1);
	U n2 = atoi_u(stok2);

	R[n1] = n2;

	return;
}

