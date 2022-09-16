#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

#include "array.h"

#define DEFAULT_SIZE 10

bool initBufff(int fd, char** buff, off_t* fSize) {
	off_t size = lseek(fd, 0L, SEEK_END);
	if(size == -1) {
		perror("Can't moove throw file");
		return false;
	}
	lseek(fd, 0L, SEEK_SET);
		
	char* newBuff = (char*)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
	if(newBuff == MAP_FAILED) {
		// perror("Can't allocate memory for file buffer");
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

void printer(array* arr, char* buff, bool all) {
	
	size_t strCount = getSize(arr) - 1;

	if(all) 
	{
		for(size_t i = 1; i <= strCount; ++i) {
			printf("%.*s\n", (int)(get(arr, i) - get(arr, i - 1) - 1),  buff + get(arr, i - 1));
		}
	}
	else 
	{
		do {
			int num;

			if(scanf("%d", &num) != 1) {
				while(getchar() != '\n') {}
				printf("Wrong argument.\n");
				continue;
			}

			if(!num){
				break;
			}

			if(num > strCount) {
				printf("There is no such string\n");
				continue;
			}

			printf("%.*s\n", (int)(get(arr, num) - get(arr, num-1) - 1),  buff + get(arr, num - 1));
		} while (true);
	}
}

void interractivePrint(array* arr, char* buff) {
	fd_set fds;
	FD_ZERO(&fds);
	int stdinFD = 0;
	FD_SET(stdinFD, &fds);	// 0 == stdin
	
	struct timeval tDat;
	tDat.tv_sec = 5;
	tDat.tv_usec = 0;

	printf("There are %d strings.\n", (int)getSize(arr) - 1);
	printf("Enter number to get string (0 to exit).\nYou hawe 5 sec to think.\n");

	int dNum = select(stdinFD + 1, &fds, NULL, NULL, &tDat);
	if(dNum == -1) {
		perror("Error while waiting for str number");
		return;
	}

	if(dNum == 0) {
		printf("Time is over! Here is the full file.\n");
		printer(arr, buff, true);

		return;
	}

	printer(arr, buff, false);

	return;
}

int main(int argc, char* argv[]) {

	if(argc != 2) {
		perror("Wrong arguments. Use:\nfinder file_name");
		
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("Can't open file");
		
		return -1;
	}

	char* buff;
	off_t size;
	if(!initBufff(fd, &buff, &size)) {
		perror("Can't init buffer");
		close(fd);
		
		return -1;
	}

	array* arr = initArray(DEFAULT_SIZE);
	if(!arr) {
		perror("Can't initialize araay structure.");
		close(fd);
		munmap(buff, size);
		
		return -1;
	}

	if(!parse(buff, size, arr)) {
		perror("Can't parse file");
		close(fd);
		destroyArr(arr);
		munmap(buff, size);
	
		return -1;
	}

	interractivePrint(arr, buff);

	destroyArr(arr);
	if(munmap(buff, size) == -1) {
		perror("Unmap err");
		return -1;
	}	
	if(close(fd) == -1) {
		perror("Close err");
		return -1;
	}

	return 0;
}
