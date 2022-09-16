#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LList.h"

#define DEFAULT_SIZE 100

void printfCustom(char* str){
	printf("%s", str);
}

int main() {
	llist list = {NULL, NULL};
	bool errFlag = false;

	do {
		char tmp[DEFAULT_SIZE];
		if(!fgets(tmp, DEFAULT_SIZE, stdin)) {
			perror("Can't read string");
			errFlag = true;
			break;
		}
		if((*tmp) == '.')
			break;
		
		size_t size = strlen(tmp) + 1;  // Including '\0'
		char* toInsert = (char*)malloc(size * sizeof(char));
		if(!toInsert) {
			perror("Failed to allocate memory for string");
			errFlag = true;
			break;
		}
		memcpy(toInsert, tmp, size);
		
		if(!LLPut(&list, toInsert)) {
			perror("Failed to create node");
			free(toInsert);
			errFlag = 1;
			break;
		}
	} while (1);

	if(errFlag) {
		LLDestroy(&list, free);
		return 1;
	}

	foreach(&list, printfCustom);
	LLDestroy(&list, free);
	return 0;
}