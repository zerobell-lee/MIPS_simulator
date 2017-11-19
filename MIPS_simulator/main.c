#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mips.h"

U PC = 0x00;
U M[M_SIZE];
U R[32] = { 0, };

int main() {
	FILE *fp;
	char command[300] = { 0, };
	char cmd0;
	char stok1[10] = { 0, };
	char stok2[10] = { 0, };
	char path[300] = { 0, };
	int bin_r = 0;
	int end = 0;

	R[29] = (M_SIZE - 1) * 4;
	R[30] = (M_SIZE - 1) * 4;

	while (1) {
		printf(">>>");
		gets_s(command, sizeof(command));
		cmd0 = command[0];

		switch (cmd0) {
		case 'l':
			end = 0;
			strtok(command, " \n");
			strcpy(path,strtok(NULL, " \n"));
			load(path, bin_r);
			continue;
		case 'm':
			end = 0;
			strtok(command, " \n");

			strcpy(stok1, strtok(NULL, " \n"));
			strcpy(stok2, strtok(NULL, " \n"));
			showmemory(stok1, stok2);
			continue;
		case 's':
			end = 0;
			if (command[1] == 'r') {
				strtok(command, " \n");

				strcpy(stok1, strtok(NULL, " \n"));
				strcpy(stok2, strtok(NULL, " \n"));
				setregister(stok1, stok2);
				continue;
			}
			else if (command[1] == '\0') {
				datapath(0);
				continue;
			}
			continue;
		case 'r':
			end = 0;
			if (command[1] == 'm') {
				bin_r = !bin_r;
				if (bin_r) printf("Read Mode : Binary\n");
				else printf("Read Mode : Text\n");
				continue;
			}
			else {
				showregister();
				continue;
			}
		case 'g':
			end = 0;
			datapath(1);
			continue;
		case 'j':
			end = 0;
			strtok(command, " \n");
			strcpy(stok1, strtok(NULL, " \n"));
			PC=atoi_u(stok1);
			continue;
		case 'x':
			if (end) exit(0);
			memset(M, 0x0, sizeof(M));
			memset(R, 0x0, sizeof(R));
			R[29] = (M_SIZE - 1) * 4;
			R[30] = (M_SIZE - 1) * 4;
			PC = 0;
			end++;
			continue;
		default:
			continue;
		}
	}
	
	return 0;

}

