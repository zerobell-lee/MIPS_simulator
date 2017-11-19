#pragma once
#define M_SIZE 1000
#define FALSE -1
#define TRUE 1
typedef unsigned int U;

extern U PC;
extern U M[M_SIZE];
extern U R[32];

void load(char path[], int bin_r);
void showmemory(char stok1[], char stok2[]);
void showregister();
void setregister(char stok1[], char stok2[]);
void advance_PC(int offset);
void datapath(int n);
void fetch_instr(U* instr);
int operation(U instr);

void lui(int rt, int offset);
void add(int rd, int rs, int rt);
void sub(int rd, int rs, int rt);
void slt(int rd, int rs, int rt);
void addi(int rt, int rs, int offset);
void slti(int rt, int rs, int offset);
void and(int rd, int rs, int rt);
void or (int rd, int rs, int rt);
void xor(int rd, int rs, int rt);
void nor(int rd, int rs, int rt);
void andi(int rt, int rs, int offset);
void ori(int rt, int rs, int offset);
void xori(int rt, int rs, int offset);
void lw(int rt, int rs, int offset);
void sw(int rt, int rs, int offset);
void jmp(U addr);
void jr(int rs);
void bltz(int rs, int offset);
void beq(int rs, int rt, int offset);
void bne(int rs, int rt, int offset);
void jal(U addr);
int syscall();

void alu(int rd, int rs, int rt, int fn);
void i_type(int op, int rs, int rt, int offset);

int cvt_arry(U num);
U cvt_addr(int num);

char DorH(char str[]);
int atoi_s(char str[]);
U atoi_u(char str[]);