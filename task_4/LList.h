#ifndef __LIST__
#define __LIST__

#include <stdbool.h>
#include <stdlib.h>

typedef void* dtype;

typedef struct node {
	dtype data;
	struct node* next;
} node;

typedef struct llist {
	node* head;
	node* last;
} llist;

node* createNode(dtype value) {
	node* nn = (node*)malloc(sizeof(struct node));
	if(!nn)
		return NULL;

	nn->data = value;
	nn->next = NULL;
	return nn;
}

bool LLPut(llist* list, dtype value) {
	node* toapp = createNode(value);
	if(!toapp)
		return  0;

	if (list->head == NULL)
		list->head = toapp;
	else
		list->last->next = toapp;
	
	list->last = toapp;
	return 1;
}

node* LLGetLastNode(llist* list) {
	return list->last;
}

dtype LLGetLastElem(llist* list) {
	return list->last->data;
}

void foreach(llist* list, void(*todo)(dtype)) {
	node* tmp = list->head;
	while (tmp != NULL) {
		todo(tmp->data );
		tmp = tmp->next;
	}
}

void LLDestroy(llist* list, void(*del)(dtype)) {
	node* head = list->head;
	while (head != NULL) {
		node* tmp = head->next;
		del(head->data);
		free(head);
		head = tmp;
	}
}

#endif