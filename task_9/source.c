#include <stdio.h> 
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	printf("Proc ID is: %d\n", (int)getpid());

	pid_t pid = fork();
    switch (pid) {
	case -1:
		perror("Fork err:");
		return -1;

	case 0:
		printf("We are in child. Proc ID is: %d\n", (int)getpid());
		printf("Cat'ting file..\n");

		execlp("cat", "cat", argv[1], NULL);
		perror("Exec err");
		return -1;

	default: 
		int stat;
		printf("We are in parent. Child pid is: %d\n", (int)pid);
		if(wait(&stat) == -1) {
			perror("Wait err:");
			return -1;
		}

		printf("\nEnding parent..\n");        
		break;
	}

	return 0;
}