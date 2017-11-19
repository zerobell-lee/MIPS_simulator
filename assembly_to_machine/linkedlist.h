#pragma once
#include <stdio.h>
#include <stdlib.h>
#ifndef __node__
#define __node__
typedef struct _node {
	char label[20];
	int row_num;
	struct _node* next;
} node;
#endif
#define TRUE 1
#define FALSE 0

typedef struct LinkedList {
	node* head;
	node* cur;
	node* last;
	int NumOfItems;
} List;

void LInit(List* plist);
int FindItem(List* plist, char label[], int* row_num);
int IsEmpty(List* plist);
void InsertItem(List* plist, char label[], int row_num);
int delAllNodes(List* plist);