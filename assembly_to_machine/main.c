#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

unsigned int to_big_endian(unsigned int num) {
	return (((num & 0xff000000) >> 24) + ((num & 0x00ff0000) >> 8) + ((num & 0x0000ff00) << 8) + ((num & 0x000000ff) << 24));
}

char DorH(char str[]) {
	if ((str[0] == '0') && (str[1] == 'x')) return 'H';
	else return 'D';
} //문자열이 16진수인지 10진수인지 판별하는 함수. 문자열이 0x로 시작하면 'H'를, 그렇지 않으면 'D'를 반환합니다.

int atoi_s(char str[]) {
	int num=0;
	if (DorH(str) == 'H') {
		int i=2;
		while (str[i] != 0) {
			if ((str[i]>='A')&&(str[i]<='F'))
				num = (num << 4) + str[i] - 'A' + 10;
			else if ((str[i] >= 'a') && (str[i] <= 'f'))
				num = (num << 4) + str[i] - 'a' + 10;
			else
				num = (num << 4) + str[i] - '0';

			i++;
		}

		return num;
	}
	else {
		num = atoi(str);
		return num;
	}
} // Signed Int로 문자열을 숫자로 변환. 문자열이 16진수인지 10진수인지를 판별하여 그에 맞는 변환과정을 거칩니다.

unsigned int atoi_u(char str[]) {
	unsigned int num = 0;
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
		unsigned int j = 1;
		for (i = strlen(str) - 1; i >= 0; i--) {
			num += ((str[i] - '0')*j);
			j *= 10;
		}
		return num;
	}
} // Unsigned Int로 문자열을 숫자로 변환. 문자열이 16진수인지 10진수인지를 판별하여 그에 맞는 변환과정을 거칩니다.

unsigned int cvt_int_u(char str[]) {
	int i = 0;
	unsigned int rint = 0;
	
	while (str[i] != 0) {
		rint = (rint << 1) + str[i] - '0';
		i++;
	}
	return rint;
}

int cvt_reg(char str[]) {
	char rstr[3] = { 0, };
	if ((str[1] >= '0') && (str[1] <= '9')) {
		rstr[0] = str[1];
		if (str[2] != ',') rstr[1] = str[2];
		return atoi(rstr);
	}
	else {
		if (str[1] == 'v') return (str[2] - '0' + 2);
		else if (str[1] == 'z') return 0;
		else if (str[1] == 'a') {
			if (str[2] == 't') return 1;
			else return (str[2] - '0' + 4);
		}
		else if (str[1] == 't') {
			if (str[2] < '8') return (str[2] - '0' + 8);
			else return (str[2] - '8' + 24);
		}
		else if (str[1] == 's') {
			if (str[2] == 'p') return 29;
			else return (str[2] - '0' + 16);
		}
		else if (str[1] == 'k') return (str[2] - '0' + 26);
		else if (str[1] == 'g') return 28;
		else if (str[1] == 'f') return 30;
		else return 31;
	}
}

void cvt_bin_r(int reg, char str[]) {
	int i;
	memset(str, 0x0, sizeof(str));
	strcat(str, "00000");
	for (i = 4; i > -1; i--) {
		str[i] = (reg&0x1) + '0';
		if (reg == reg >> 1) reg = reg / 2;
		else reg = reg >> 1;
	}
}

void cvt_bin_i(int imm, char str[]) {
	int i;
	memset(str, 0x0, sizeof(str));
	for (i = 15; i > -1; i--) {
		str[i] = (imm & 0x1) + '0';
		imm = imm >> 1;
	}
}

void cvt_address(int row_num, char str[]) {
	int i;
	memset(str, 0x0, sizeof(str));
	for (i = 25; i > -1; i--) {
		str[i] = (row_num & 0x1) + '0';
		if (row_num == row_num>>1) row_num = row_num / 2;
		else row_num = row_num>>1;
	}

}


int main() {
	FILE* fp;
	FILE* fp2;
	FILE* fp3;
	FILE* fp4;

	char path[100] = { 0, };
	char input[300] = { 0, };
	char stok1[30] = { 0, };
	char stok2[30] = { 0, };
	char stok3[30] = { 0, };
	char stok4[30] = { 0, };
	char output[33] = { 0, };
	int row_num = -1;
	int c_row = -1;
	int rs;
	int rt;
	int rd;
	int imm;
	int address;
	int fn;
	int op;

	unsigned int output2[1000] = {0,};
	unsigned int output3[1000] = { 0, };
	char bin_w=0;

	List LabelList;
	LInit(&LabelList);


	printf("%c >>>load : ", bin_w);
	gets_s(path,sizeof(path));
	fp = fopen(path, "rt");

	if (fp == NULL) {
		printf("File doesn't exist!!\n");
		exit(-1);
	}
	while (fgets(input,sizeof(input),fp) != NULL) {
		row_num++;
		
		strcpy(stok1, strtok(input, " \t\n"));
		if (stok1[strlen(stok1) - 1] == ':') {
			stok1[strlen(stok1) - 1] = '\0';
			InsertItem(&LabelList, stok1, row_num);
		}
		memset(input, 0x00, sizeof(input));
	}
	fclose(fp);
	memset(input, 0x00, sizeof(input));

	fp = fopen(path, "rt");
	fp2 = fopen("result.txt", "wt");
	fp3 = fopen("result_little_endian.bin", "wb");
	fp4 = fopen("result_big_endian.bin", "wb");

	while (fgets(input, sizeof(input), fp) != NULL) {
		c_row++;
		memset(stok1, 0x0, sizeof(stok1));
		strcat(stok1, strtok(input, " \t\n"));
		if (stok1[strlen(stok1) - 1] == ':') {
			memset(stok1, 0x0, sizeof(stok1));
			strcat(stok1, strtok(NULL, " \t\n"));
		}
		if (strcmp(stok1, "lui") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			memset(stok3, 0x0, sizeof(stok2));
			strcat(stok3, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok2);
			imm = atoi_s(stok3);
			strcat(output, "00111100000");
			cvt_bin_r(rt, stok2);
			cvt_bin_i(imm, stok3);
			strcat(output, stok2);
			strcat(output, stok3);
		}
		else if (strcmp(stok1, "add") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			memset(stok3, 0x0, sizeof(stok3));
			strcat(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000100000");
		}
		else if (strcmp(stok1, "sub") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			memset(stok3, 0x0, sizeof(stok3));
			strcat(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000100010");
		}
		else if (strcmp(stok1, "slt") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			memset(stok3, 0x0, sizeof(stok3));
			strcat(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			memset(stok2, 0x0, sizeof(stok2));
			memset(stok3, 0x0, sizeof(stok3));
			memset(stok4, 0x0, sizeof(stok4));
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000101010");
		}
		else if (strcmp(stok1, "addi") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			memset(stok3, 0x0, sizeof(stok3));
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok2, strtok(NULL, " \t\n"));
			strcat(stok3, strtok(NULL, " \t\n"));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok3);
			imm = atoi_s(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);
			strcat(output, "001000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "slti") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			memset(stok3, 0x0, sizeof(stok3));
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok2, strtok(NULL, " \t\n"));
			strcat(stok3, strtok(NULL, " \t\n"));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok3);
			imm = atoi_s(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);
			strcat(output, "001010");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "and") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			memset(stok3, 0x0, sizeof(stok3));
			strcat(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000100100");
		}
		else if (strcmp(stok1, "or") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			memset(stok3, 0x0, sizeof(stok3));
			strcat(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000100101");
		}
		else if (strcmp(stok1, "xor") == 0) {
			memset(stok2, 0x0, sizeof(stok2));
			strcat(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			memset(stok3, 0x0, sizeof(stok3));
			strcat(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			memset(stok4, 0x0, sizeof(stok4));
			strcat(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000100110");
		}
		else if (strcmp(stok1, "nor") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			rd = cvt_reg(stok2);
			strcpy(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok3);
			strcpy(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_r(rd, stok4);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
			strcat(output, "00000100111");
		}
		else if (strcmp(stok1, "andi") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok3);
			imm = atoi_s(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);
			strcat(output, "001100");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "ori") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok3);
			imm = atoi_s(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);

			strcat(output, "001101");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "xori") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok4, strtok(NULL, " \t\n"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok3);
			imm = atoi_s(stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);
			strcat(output, "001110");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "lw") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(stok3, "()"));
			strcpy(stok4, strtok(NULL, "()"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok4);
			imm = atoi_s(stok3);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);
			strcat(output, "100011");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "sw") == 0) {
			
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(stok3, "()"));
			strcpy(stok4, strtok(NULL, "()"));
			rt = cvt_reg(stok2);
			rs = cvt_reg(stok4);
			imm = atoi_s(stok3);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			cvt_bin_i(imm, stok4);
			strcat(output, "101011");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "j") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			FindItem(&LabelList, stok2, &row_num);
			cvt_address(row_num, stok2);
			strcat(output, "000010");
			strcat(output, stok2);
		}
		else if (strcmp(stok1, "jr") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok2);
			cvt_bin_r(rs, stok2);
			strcat(output, "000000");
			strcat(output, stok2);
			strcat(output, "000000000000000001000");
		}
		else if (strcmp(stok1, "bltz") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok2);
			FindItem(&LabelList, stok3, &row_num);
			cvt_bin_i(row_num - c_row, stok3);
			cvt_bin_r(rs, stok2);
			strcat(output, "000001");
			strcat(output, stok2);
			strcat(output, "00000");
			strcat(output, stok3);
		}
		else if (strcmp(stok1, "beq") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok4, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok2);
			rt = cvt_reg(stok3);
			FindItem(&LabelList, stok4, &row_num);
			cvt_bin_i((row_num - c_row), stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			strcat(output, "000100");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "bne") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			strcpy(stok3, strtok(NULL, " \t\n"));
			strcpy(stok4, strtok(NULL, " \t\n"));
			rs = cvt_reg(stok2);
			rt = cvt_reg(stok3);
			FindItem(&LabelList, stok4, &row_num);
			cvt_bin_i(row_num - c_row, stok4);
			cvt_bin_r(rs, stok2);
			cvt_bin_r(rt, stok3);
			strcat(output, "000101");
			strcat(output, stok2);
			strcat(output, stok3);
			strcat(output, stok4);
		}
		else if (strcmp(stok1, "jal") == 0) {
			strcpy(stok2, strtok(NULL, " \t\n"));
			FindItem(&LabelList, stok2, &row_num);
			cvt_address(row_num, stok2);
			strcat(output, "000011");
			strcat(output, stok2);
		}
		else if (strcmp(stok1, "syscall") == 0) {
			strcat(output, "00000000000000000000000000001100");
		}
		else if ((strcmp(stok1, "nop") == 0) || (strcmp(stok1, "") == 0)) {
			strcat(output, "00000000000000000000000000000000");
		}
		fprintf(fp2, "%s\n", output); 
		output2[c_row] = cvt_int_u(output);
		output3[c_row] = to_big_endian(output2[c_row]);
		memset(output, 0x0, sizeof(output));
	}

	fwrite(output2, sizeof(output2) / sizeof(unsigned int), 1, fp3);
	fwrite(output3, sizeof(output3) / sizeof(unsigned int), 1, fp4);
	fclose(fp2);
	fclose(fp);
	fclose(fp3);
	fclose(fp4);
	delAllNodes(&LabelList);
	return 0;
}