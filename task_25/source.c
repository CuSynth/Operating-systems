#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SZ 128


int main(int argc, char *argv[]) {
	char buff[BUFF_SZ] = "This is the string to be PrInTeD!";
	int pipes[2];
	
	if(pipe(pipes) == -1) {
		perror("Pipe err");
		return -1;
	}

    pid_t pid = fork();
    
	switch (pid) {
	case -1:
		perror("Fork err");
		close(pipes[0]);
		close(pipes[1]);
		return -1;

	case 0:	// 1st child, sending process
		if(close(pipes[0]) == -1) {
			perror("Close pipes[0] err in sending process");
			return -1;
		}
		
		if (write(pipes[1], buff, strlen(buff) + 1) == -1) {
			perror("Write err");
			// close(pipes[1]);
			return -1;
		}

		if(close(pipes[1]) == -1) {
			perror("Close pipes[1] err in sending process");
			return -1;
		}

		return 0;

	default:

		pid_t nPid = fork();
		switch (nPid) {
		case -1:
			perror("Second fork err");
			return -1;
			break;
		
		case 0:	// 2nd child, receiving process
			if(close(pipes[1]) == -1) {
				perror("Close pipes[1] err in receiving procecc");
				return -1;
			}

			int size;
			while((size = read(pipes[0], buff, BUFF_SZ))) {
				for (int i = 0; i < size; i++)
					buff[i] = toupper(buff[i]);
				write(STDOUT_FILENO, buff, size);
			}
			write(STDOUT_FILENO, "\n", 1);
			
			if(close(pipes[0]) == -1) {
				perror("Close pipes[0] err in receiving procecc");
				return -1;
			}
			
			return 0;
			
		default:	// Parent
			break;
		}

		if(close(pipes[0]) == -1) {
			perror("Close pipes[0] err in parent");
			return -1;
		}

		if(close(pipes[1]) == -1) {
			perror("Close pipes[1] err in parent");
			return -1;
		}
		
		int stat;
		if(wait(&stat) == -1) {
			perror("Wait err");
			return -1;
		}		
   	
	   	return 0;
	}
}