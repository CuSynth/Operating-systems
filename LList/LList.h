#pragma once
#include <stdbool.h>

typedef int dtype;

typedef struct node* node;

void LLDestroy(node);

unsigned LLLength(node);

node LLPrepend(node, dtype);

node LLAppend(node, dtype);

dtype LLGet(node, unsigned);

node LLRemove(node, unsigned);

dtype LLGetLast(node);

int LLFind(node, dtype);

node LLRemoveFirst(node, dtype);

node LLRemoveAll(node, dtype);

node LLCopy(node);

node LLConcat(node, node);

void Foreach(node, void (*f)(dtype, void*));

int FindCustom(node, bool(dtype));