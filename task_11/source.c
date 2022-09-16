#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern char **environ;

int execvpe(const char* file, char* const argv[], char* const envp[]) {
	char **prevEnv = environ;
	environ = (char**)envp;

	execvp(file, argv);
	environ = prevEnv;
	return -1;

}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("Wrong arguments. Use:\n%s ./program_to_start\n", argv[0]);
		return 1;
	}

	char* customArgs[] = {"Arg_0", "Arg_1", "Arg_2", NULL};
	char* customEnv[] = {"param_A=A", "param_B=B", "param_C=C", "param_A=C", "param_B", NULL};

	execvpe(argv[1], customArgs, customEnv);
	perror("Execvpe error");
	return -1;
	
}
