#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void LInit(List* plist) {
	plist->cur = NULL;
	plist->head = NULL;
	plist->last = NULL;
	plist->NumOfItems = 0;
};
int FindItem(List* plist, char label[], int* row_num) {
	node* retNode;
	if (IsEmpty(plist)) {
		memset(label, 0x00, sizeof(label));
		*row_num = -1;
		return FALSE;
	}
	else {
		retNode = plist->head;
		if (retNode->next == NULL) {
			if (strcmp(label, retNode->label) == 0) {
				*row_num = retNode->row_num;
				return TRUE;
			}
			else {
				memset(label, 0x00, sizeof(label));
				*row_num = -1;
				return FALSE;
				
			}
		}
		else {
			if (strcmp(label, retNode->label) == 0) {
				*row_num = retNode->row_num;
				return TRUE;
			}
			else retNode = retNode->next;
			while (retNode->next != NULL) {
				if (strcmp(label, retNode->label) == 0) {
					*row_num = retNode->row_num;
					return TRUE;
				}
				else {
					retNode = retNode->next;
				}
			}
			if (strcmp(label, retNode->label) == 0) {
				*row_num = retNode->row_num;
				return TRUE;
			}
			else {
				memset(label, 0x00, sizeof(label));
				*row_num = -1;
				return FALSE;
				
			}
		}
	}
};
int IsEmpty(List* plist) {
	if (plist->head == NULL) return TRUE;
	else return FALSE;
};
void InsertItem(List* plist, char label[], int row_num) {
	node* NewNode = malloc(sizeof(node));
	strcpy(NewNode->label, label);
	NewNode->row_num = row_num;
	NewNode->next = NULL;
	if (IsEmpty(plist)) {
		plist->head = NewNode;
		plist->cur = NewNode;
	}
	else {
		plist->cur->next = NewNode;
		plist->cur = NewNode;
	}
	plist->NumOfItems++;
};
int delAllNodes(List* plist) {
	node* delNode;
	if (IsEmpty(plist)) return FALSE;
	else {
		delNode = plist->head;
		while (delNode->next != NULL) {
			plist->head = delNode->next;
			free(delNode);
			delNode = plist->head;
		}
		free(delNode);
		LInit(plist);
	}
};