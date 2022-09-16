#include <stdio.h> 



void ParseAndPrint(int stat) {
    printf("Status is:\n");

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
	char buff[] = "This is the string to be PrInTeD!";

	FILE* f = popen("./editor1", "w");
	if(!f) {
		perror("Popen err");
		return -1;
	}

	fprintf(f, "%s", buff);
	
	int ret = pclose(f);

	ParseAndPrint(ret);
	if(ret == -1) {
		perror("Pclose err");
		return -1;
	}

    return 0;
}