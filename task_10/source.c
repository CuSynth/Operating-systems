#include <stdio.h> 
#include <unistd.h>
#include <sys/wait.h>

void ParseAndPrint(pid_t pid, int stat) {
	printf("[%d] status is:\n", (int)pid);

	if(WIFEXITED(stat)) {
		printf("Process terminated normally.\n");
	}
	
	if(WIFEXITED(stat) && WEXITSTATUS(stat)) {
		printf("Process ended throw exit code %d\n", WEXITSTATUS(stat));
	}

	// if(WIFSIGNALED(stat)) {
	// 	printf("Process terminated throw the signal\n");
	// }

	if(WIFSIGNALED(stat) && WTERMSIG(stat)) {
		printf("Process terminated throw the signal %d\n", WTERMSIG(stat));
	}

	// if(WIFSTOPPED(stat)) {
	// 	printf("Process is stopped\n");
	// }

	if(WIFSTOPPED(stat) && WSTOPSIG(stat)) {
		printf("Process is stopped py signal %d\n", WSTOPSIG(stat));
	}

	if(WCOREDUMP(stat)) {
		printf("Child produced a core dump %d\n", WCOREDUMP(stat));
	}

	if(WIFCONTINUED(stat)) {
		printf("Child process was resumed\n");
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Wrong arguments. Use:\n%s program args..\n", argv[0]);
		// perror("Wrong arguments. Use:\n_task program args..");
		
		return -1;
	}

	pid_t pid = fork();

	switch (pid) {
	case -1:
		perror("Fork err");
		return -1;

	case 0:
		execvp(argv[1], argv + 1);
		perror("Exec err");
		return -1;
	
	default: 
		int stat;
		if(wait(&stat) == -1) {
			perror("Wait err");
			return -1;
		}
		ParseAndPrint(pid, stat);    
	}

	return 0;
}