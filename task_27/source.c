#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "../tmp_/ParseAndPrint.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Wrong arguments. Use:\n%s file_name\n", argv[0]);
		return 1;
	}

    FILE* file = fopen(argv[1], "r");
    if(!file){
        perror("Open err");
        return -1;
    }

    char* _grp = "grep ^$ ";
    char* _wc = " | wc1 -l";
    char* cmd = (char*)malloc((strlen(_grp) + strlen(_wc) + strlen(argv[1])) + 1);
    cmd[0] = '\0';
    strcat(cmd, _grp);
    strcat(cmd, argv[1]);
    strcat(cmd, _wc);

    FILE* counter = popen(cmd, "r");

    int count;
    fscanf(counter, "%d", &count);
    printf("Count: %d\n", count);

	int ret = pclose(counter);

	ParseAndPrint(ret);
	if(ret == -1) {
		perror("Pclose err");
		return -1;
	}
    printf("Ret is: %d\n", ret);
    
    fclose(file);
    return 0;
}