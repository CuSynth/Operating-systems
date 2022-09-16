#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "LList.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

void count(dtype data, void *num) {
	(*(unsigned*)num)++;
}

node MakeList() {
	node mylist = NULL;
	node nml = NULL;
	nml = LLPrepend(mylist, 30);
	LLDestroy(mylist);
	mylist = LLPrepend(nml, 20);
	LLDestroy(nml);
	nml = LLPrepend(mylist, 10);
	LLDestroy(mylist);
	return nml;
}

bool TestCopy(node first, node second) {
	if (LLLength(first) != LLLength(second))
		return false;

	for (int i = 0; i < LLLength(first); i++)
	{
		if (LLGet(first, i) != LLGet(second, i))
			return false;
	}

	return true;
}

bool IsNegative(dtype val) {
	return val < 0;
}

int main() {
	node mylist = MakeList();
	assert(LLLength(mylist) == 3);

	assert(LLGet(mylist, 0) == 10);
	assert(LLGet(mylist, 1) == 20);
	assert(LLGet(mylist, 2) == 30);
	assert(LLGet(mylist, 3) == NULL);
	assert(LLGetLast(mylist) == 30);

	node nml = LLAppend(mylist, 50);
	LLDestroy(mylist);

	assert(LLLength(nml) == 4);
	assert(LLGet(nml, 3) == 50);
	assert(LLGet(nml, 0) == 10);
	
	assert(LLFind(nml, 20) == 1);
	assert(LLFind(nml, 256) == -1);
	
	mylist = LLRemove(nml, 1);
	assert(LLFind(mylist, 20) == -1);
	LLDestroy(mylist);

	for (int i = 0; i < 5; i++) {
		mylist = LLPrepend(nml, 1);
		LLDestroy(nml);
		nml = LLAppend(mylist, 1);
		LLDestroy(mylist);
	}

	assert(LLLength(nml) == 14);
	mylist = LLRemoveAll(nml, 1);
	LLDestroy(nml);
	assert(LLFind(mylist, 1) == -1);
	assert(LLLength(mylist) == 4);

	nml = LLCopy(mylist);
	assert(TestCopy(mylist, nml) == true);

	node conclist =	LLConcat(mylist, nml);
	assert(LLLength(conclist) == 8);
	LLDestroy(conclist);
	LLDestroy(nml);

	nml = LLAppend(mylist, -5);
	assert(FindCustom(nml, IsNegative) == 4);	

	LLDestroy(mylist);

	unsigned counter = 0;
	Foreach(nml, count, &counter);
	assert(counter == 5);

	LLDestroy(nml);

	_CrtDumpMemoryLeaks();
}