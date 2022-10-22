#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/time.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

bool lockAndRead(int fd) {
	struct flock file_info;
	memset(&file_info, 0, sizeof(file_info));

	file_info.l_type = F_WRLCK;
	file_info.l_whence = SEEK_SET;
	file_info.l_start = 0;
	file_info.l_len = 0;

	int returned = fcntl(fd, F_GETLK, &file_info);
	if (returned == -1)
	{
		perror("fcntl(3) error");
		int close_returned = close(fd);
		if (close_returned == -1)
		{
			perror("close(1) error");
		}
		return 2;
	}
	if (file_info.l_type != F_UNLCK) {
		printf("File is already locked by pid=%d\n", file_info.l_pid);
		return 3;
	}
	file_info.l_type = F_WRLCK;
	returned = fcntl(fd, F_SETLK, &file_info);

	if (returned == -1)
	{
		perror("fcntl(..,F_SETLK,..) error");
		return 4;
	}
	int sysRes = system("nano data");

	if (sysRes == -1)
	{
		perror("system(1) error");
		int close_returned = close(fd);
		if (close_returned == -1)
		{
			perror("close(1) error");
		}
		flock(fd, LOCK_UN);
		return 5;
	}
	return 0;
}

int main(int argc, char* argv[]) {

	if(argc != 2) {
		perror("Wrong arguments. Use:\neditor file_name\n");
		
		return 1;
	}

	int fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		perror("Can't open file");
		
		return 1;
	}


	return lockAndRead(fd);
}
