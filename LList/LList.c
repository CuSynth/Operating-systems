#include <stdbool.h>
#include <stdlib.h>

#include "LList.h"


#define NULL_if_N(a) if(a == NULL) return NULL
#define CL_if_N(a, b) if(a == NULL) { LLDestroy(b); return NULL; }

typedef struct node {
	dtype data;
	struct node* next;
} *node;

void LLDestroy(node head) {
	while (head != NULL) {
		node tmp = head->next;
		free(head);
		head = tmp;
	}
}

unsigned LLLength(node head) {
	unsigned counter = 0;
	while (head != NULL) {
		counter++;
		head = head->next;
	}

	return counter;
}

node CreateNode(dtype value, node next) {
	node nn = malloc(sizeof(struct node));
	NULL_if_N(nn);
	nn->data = value;
	nn->next = next;
	return nn;
}

node LLCopy(node head) {
	NULL_if_N(head);

	node nn = CreateNode(head->data, NULL);
	NULL_if_N(nn);
	head = head->next;
	node tmp = nn;

	while (head != NULL) {

		tmp->next = CreateNode(head->data, NULL);
		CL_if_N(tmp->next, nn);
		tmp = tmp->next;
		head = head->next;
	}

	return nn;
}

node LLPrepend(node head, dtype value) {
	node nn = CreateNode(value, NULL);
	NULL_if_N(nn);
	if (head == NULL)
		return nn;	
	node nl = LLCopy(head);
	CL_if_N(nl, nn);	
	nn->next = nl;

	return nn;
}

node LLAppend(node head, dtype value) {
	if (head == NULL) {
		//return CreateNode(value, NULL);
		node nn = CreateNode(value, NULL);
		NULL_if_N(nn);
		return nn;
	}
	node toapp = CreateNode(value, NULL);
	NULL_if_N(toapp);

	node nn = CreateNode(head->data, NULL);
	CL_if_N(nn, toapp);
	head = head->next;
	node tmp = nn;
	
	while (head != NULL) {	// Тут можно чререз LLCopy, но так быстрее
		tmp->next = CreateNode(head->data, NULL);
		CL_if_N(tmp->next, nn);	// Чистим за собой при неудаче
		tmp = tmp->next;
		head = head->next;
	}

	tmp->next = toapp;
	return nn;
}

dtype LLGet(node head, unsigned pos) {
	NULL_if_N(head);

	for (int i = 0; i < pos && head != NULL; i++)
		head = head->next;

	NULL_if_N(head);
	return head->data;
}

 node LLRemove(node head, unsigned pos) {
	NULL_if_N(head);
	if (pos == 0) {
		return LLCopy(head->next);
	}

	node nn = CreateNode(head->data, NULL);
	NULL_if_N(nn);
	head = head->next;
	node tmp = nn;
	unsigned counter = 0;
	while (head != NULL) {
		if (counter++ == pos - 1) {

			head = head->next;
			continue;
		}
		tmp->next = CreateNode(head->data, NULL);
		tmp = tmp->next;
		CL_if_N(tmp, nn);
		head = head->next;
	}
	return nn;
}

dtype LLGetLast(node head) {
	NULL_if_N(head);
	while (head->next != NULL)
		head = head->next;

	return head->data;
}

int LLFind(node head, dtype data) {
	if (head == NULL)
		return -1;

	int counter = 0;

	while (head != NULL && head->data != data) {
		counter++;
		head = head->next;
	}

	if (head == NULL)
		return -1;
	return counter;
}

node LLRemoveFirst(node head, dtype value) {
	//return LLRemove(head, LLFind(head, value));	// Долго
	NULL_if_N(head);

	if (value == head->data) {
		return LLCopy(head->next);
	}
	 
	node nn = CreateNode(head->data, NULL);
	NULL_if_N(nn);
	head = head->next;
	node tmp = nn;
	bool flag = false;

	while (head != NULL) {
		if (head->data == value && flag == false) {
			head = head->next;
			flag = true;
			continue;
		}

		tmp->next = CreateNode(head->data, NULL);
		tmp = tmp->next;
		CL_if_N(tmp, nn);
		head = head->next;
	}

	return nn;
}

node LLRemoveAll(node head, dtype value) {
	NULL_if_N(head);

	while (head->data == value) {
		head = head->next;
		NULL_if_N(head);
	}

	node nn = CreateNode(head->data, NULL);
	NULL_if_N(nn);
	node tmp = nn;
	head = head->next;

	while (head != NULL) {

		if (head->data == value) {
			head = head->next;
			continue;
		}

		tmp->next = CreateNode(head->data, NULL);
		tmp = tmp->next;
		CL_if_N(tmp, nn);
		head = head->next;
	}
	return nn;
}

node LLConcat(node fhead, node shead) {
	if (fhead == NULL && shead == NULL)
		return NULL;
	else if (fhead == NULL && shead != NULL)
		return LLCopy(shead);
	else if (shead == NULL && fhead != NULL)
		return LLCopy(fhead);

	node nn = CreateNode(fhead->data, NULL);
	NULL_if_N(nn);	
	fhead = fhead->next;
	node tmp = nn;

	while (fhead != NULL)
	{
		tmp->next = CreateNode(fhead->data, NULL);
		tmp = tmp->next;
		CL_if_N(tmp, nn);
		fhead = fhead->next;
	}
	tmp->next = LLCopy(shead);
	return nn;
}

void Foreach(node head, void(*todo)(dtype, void*), void* extra_data) {

	while (head != NULL) {
		todo(head->data, extra_data);
		head = head->next;
	}
}

int FindCustom(node head, bool(*Cmpf)(dtype)) {
	if (head == NULL)
		return -1;

	int counter = 0;

	while (head != NULL && Cmpf(head->data) == false) {
		counter++;
		head = head->next;
	}

	if (head == NULL)
		return -1;
	return counter;
}