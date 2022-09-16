#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int execvpe(const char* file, char* const argv[], char* const envp[]) {
	if(clearenv()) {	// очищаем переменную extern char **environ
		perror("Environment clear err");
		return -1;
	}

	for (size_t i = 0; envp[i]; ++i) {	// Меняем окружение на необходимое
		if(putenv(envp[i])) {
			perror("Putenv error");
			return -1;
		}
	}

	execvp(file, argv);	// дочерний процесс наследует(забирает) текщее окружение, + примет ARGV в качестве аргументов
}

int main(int argc, char* argv[]) {	// Just for test. Запускать с informer в качестве аргумента
	if(argc != 2) {
		printf("Wrong arguments. Use:\n%s ./program_to_start\n", argv[0]);
		
		return -1;
	}

	char* customArgs[] = {"Arg_0", "Arg_1", "Arg_2", NULL};
	char* customEnv[] = {"param_A=A", "param_B=B", "param_C=C", "param_A=C", "param_B", NULL};
	// Задаем параметры, меняем значение параметра А на B; удаляем параметр B 

	if(execvpe(argv[1], customArgs, customEnv)) {	// В случае успеха - не вернет управление
		perror("Execvpe error");
		return -1;
	}

	// return 0;	
}
