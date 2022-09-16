#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <unistd.h>

#include "array.h"

#define DEFAULT_SIZE 10



bool initBufff(int fd, char** buff, off_t* fSize) {
	off_t size = lseek(fd, 0L, SEEK_END);
	if(size == -1) {
		// perror("Can't moove throw file");
		return false;
	}
	lseek(fd, 0L, SEEK_SET);
		
	char* newBuff = (char*)malloc( (size + 1) * sizeof(char) );
	if(!newBuff) {
		// perror("Can't allocate memory for file buffer");
		return false;
	}

	newBuff[size] = '\0';
	if(read(fd, newBuff, size) == -1) {
		// perror("Cant read file");
		free(newBuff);
		return false;
	}

	*fSize = size;
	*buff = newBuff;
	return true;
}

bool parse(char* buff, off_t buffSize, array* arr) {
	if(!put(arr, 0)) {
		return false;
	}

	for (off_t i = 0; i < buffSize; ++i) {
		if(buff[i] == '\n') {
			if(!put(arr, i + 1)) {
				return false;
			}
		}
	}

	return put(arr, buffSize + 1);
}

void interractivePrint(array* arr, char* buff) {
	size_t strCount = getSize(arr) - 1;
	printf("There are %d strings.\n", strCount);
	printf("Enter number to get string (0 to exit).\n");

	do {
		int num;

		if(scanf("%d", &num) != 1) {
			while(getchar() != '\n') {}
			printf("Wrong argument.\n");
			continue;
		}

		if(!num) {
			break;
		}
		
		if(num > strCount) {
			printf("There is no such string\n");
			continue;
		}

		printf("%.*s\n", get(arr, num) - get(arr, num-1) - 1,  buff + get(arr, num - 1));
	} while (1);
}


int main(int argc, char* argv[]) {

	if(argc != 2) {
		perror("Wrong arguments. Use:\nfinder file_name");

		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("Can't open file");

		return 1;
	}

	char* buff;
	off_t size;
	if(!initBufff(fd, &buff, &size)) {
		perror("Can't init buffer");
		close(fd);
		
		return 1;
	}

	array* arr = initArray(DEFAULT_SIZE);
	if(!arr) {
		perror("Can't initialize araay structure.");
		close(fd);
		free(buff);
		
		return 1;
	}

	if(!parse(buff, size, arr)) {
		perror("Can't parse file");
		close(fd);
		destroyArr(arr);
		free(buff);
	
		return 1;
	}
	
 	interractivePrint(arr, buff);

	close(fd);
	destroyArr(arr);
	free(buff);
	
	return 0;
}
