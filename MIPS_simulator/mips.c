#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char DorH(char str[]) {
	if ((str[0] == '0') && (str[1] == 'x')) return 'H';
	else return 'D';
} //문자열이 16진수인지 10진수인지 판별하는 함수. 문자열이 0x로 시작하면 'H'를, 그렇지 않으면 'D'를 반환합니다.

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
} // Signed Int로 문자열을 숫자로 변환. 문자열이 16진수인지 10진수인지를 판별하여 그에 맞는 변환과정을 거칩니다.

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
} // Unsigned Int로 문자열을 숫자로 변환. 문자열이 16진수인지 10진수인지를 판별하여 그에 맞는 변환과정을 거칩니다.

int cvt_arry(U num) {
	if ((num % 4) != 0) {
		printf("exception occurred\n");
		return FALSE;
	}
	return (num / 4);
} // 4의 배수를 4로 나누어주는 함수. 만일 4의 배수가 아니라면 exception occurred라는 메시지와 함께 FALSE를 반환합니다.

U cvt_addr(int num) {
	U num1 = num;
	return num1<<2;
} // 주소에 비트 00을 붙여주는 함수. jump나 branch 명령어에 활용합니다.

void advance_PC(int offset) {
	PC += offset;
} // offset만큼을 PC에 가산합니다. 보통은 advance_PC(4)의 형태로 4씩 더해줍니다.

//Inst 처리 관련 함수들
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
} // Offset이 signed int로 반환됩니다. sign-extention이 일어납니다.
U getOffset_u(unsigned int instr) {
	return ((instr << 16) >> 16);
} // 이 함수를 사용하면 Offset을 unsigned int로 반환합니다. 즉 zero-extention 상태로 반환을 하게 됩니다.
U getAddr(unsigned int instr) {
	return ((instr << 6) >> 6);
}

//DataPath와 관련된 함수
void fetch_instr(U* instr) {
	*instr = M[cvt_arry(PC)];
} //PC를 1/4한 값을 M 배열에서 찾아서 instr에 입력합니다.

int operation(U instr) {

	int op = getOp(instr); // 먼저 instr의 op를 취득하여 판별합니다.
	if (op == 0) { // op가 0이라면 ALU를 거치는 명령어라는 의미입니다. 이 명령어는 전부 R-Type입니다.
		int rd = getRd(instr); //그러므로 rd, rs, rt, fn을 취득할 필요가 있습니다.
		int rs = getRs(instr);
		int rt = getRt(instr);
		int fn = getFn(instr);
		alu(rd, rs, rt, fn);  //나머지는 ALU() 함수가 알아서 처리할 것입니다.
		return TRUE;
	}
	else if (op == 2) { // j 명령어는 I-Type도 R-Type도 아니므로 따로 처리합니다.
		U addr = getAddr(instr);
		addr = cvt_addr(addr);
		jmp(addr);
		return TRUE;
	}
	else if (op == 3) { // jal도 마찬가지로 따로 처리했습니다.
		U addr = getAddr(instr);
		addr = cvt_addr(addr);
		jal(addr);
		return TRUE;
	}
	else { // 위의 R-Type과 J-Type 명령어를 제하면 전부 I-Type 명령어만 남으므로 else 문에서 처리하도록 합니다.
		int rs = getRs(instr);
		int rt = getRt(instr);
		int offset = getOffset(instr);
		i_type(op, rs, rt, offset); //I-Type 명령어에서 알아서 처리합니다.
		return TRUE;
	}
}


void datapath(int n) { //Step 명령어와 Go 명령어의 차이를 두기 위하여 int n을 따로 받아서 처리하도록 하였습니다.
	do {
		U instr;
		fetch_instr(&instr); //PC로부터 instr을 받아옵니다.
		if (instr == 0) { //만일 32비트 모두가 0이라면, 즉 nop이라면 별도의 처리 구간을 넣어줍니다.
			char ch;
			printf("No Instruction!! Wanna Continue?(y/n)");
			ch = getchar();
			if ((ch == 'y') || (ch == 'Y')) {
				advance_PC(4); //진행을 원한다면 다음 줄로 넘어가게 되고, 아닐 경우는 datapath가 종료됩니다.
				continue;
			}
			else {
				break;
			}
		}
		else if (instr == 0x0000000c) { 
			break; //syscall을 만나면 이 datapath는 종료됩니다.
		}
		else operation(instr); //nop도 syscall도 아니라면, operation 함수를 통하여 instr을 실행해줍니다.
	} while (n); 
	/*
	만일 int n이 0이었다면 do { } 안에 있는 명령어를 단 한번만 수행하고 종료가 됩니다.
	그러나 n이 0이 아닌 다른 숫자라면 syscall을 만날 때까지 무한 반복을 수행하게 됩니다.
	*/
}


void load(char path[], int bin_r) { //하드로부터 binary file이나 text file을 불러오는 함수입니다.
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
		FILE* fp = fopen(path, "rt"); //binary file이라면 rb를 하여도 되지만, 엔터나 공백의 처리를 위해서 rt 모드로 읽었습니다.
		if (fp == NULL) {
			printf("File Doesn't Exist!\n");
			exit(-1);
		}
		U instr;
		U digit;

		int i = 0;
		int j;
		int k = 0;

		checker = fgetc(fp); //먼저 파일 포인터로부터 한 글자를 읽어옵니다. 더 이상 읽어올 글자가 없다면 -1이 반환됩니다.
		while (checker != -1) { //읽어올 글자가 없는 상황이 올 때까지 계속해서 다음 반복문을 반복합니다.
			if (checker == '\n') {
				checker = fgetc(fp); //개행문자를 만났을 경우엔 무시하고 다음 글자를 읽습니다. 이후 반복문을 다시 시작합니다.
				continue;
			}
			input[i] = checker; //읽어온 문자가 개행문자가 아니었다면 input 배열에 해당 글자를 대입합니다.
			if (i == 31) { //배열 32개가 전부 꽉 찼다면, 이를 instr로 바꿔줘야합니다.
				instr = 0;
				for (j = 0; j < 32; j++) {
					if (input[j] == '0') digit = 0;
					else digit = 1;
					instr += digit << (31 - j); //32개의 0과 1을 Unsigned Int 형태로 변환시킵니다.
				}
				M[k] = instr; //메모리를 뜻하는 M 배열에 해당 instr을 대입합니다.
				k++; //다음 메모리를 읽을 준비를 합니다.
				i = 0;

			}
			else {
				i++; //아직 배열이 32개가 전부 차지 않았다면 패스합니다.
			}
			checker = fgetc(fp); //다시 다음 글자를 읽어들입니다.
		}
		fclose(fp);
	}
}

void showmemory(char stok1[], char stok2[]) { //stok1부터 stok2에 해당하는 범위의 메모리를 표시합니다.
	U n1 = atoi_u(stok1);
	U n2 = atoi_u(stok2);

	int i = cvt_arry(n1);
	if (i == FALSE) return;
	while (i <= cvt_arry(n2)) {
		printf("[%08x] : 0x%08x\n", cvt_addr(i), M[i]); //stok은 10진수든 16진수든, 4의 배수이기만 하다면 상관 없습니다.
		i++;
	}
}

void showregister(void) { //32개의 레지스터를 전부 표시하는 함수입니다. PC도 표시합니다.
	int i = 0;
	for (i = 0; i < 32; i++)
		printf("R[%d] : 0x%08x\n", i, R[i]);
	printf("PC : 0x%08x\n", PC);

}

void setregister(char stok1[], char stok2[]) { //특정 레지스터의 값을 바꾸어주는 함수입니다.
	int n1 = atoi_s(stok1);
	U n2 = atoi_u(stok2);

	R[n1] = n2;

	return;
}

