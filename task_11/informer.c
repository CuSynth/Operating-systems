#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[]) {
    
    printf("Arguments:\n");
    for (int n = 0; n < argc; ++n) {
        printf("\t%s\n", argv[n]);
    }

    printf("\nEnv:\n");
    for (char **p = environ; *p; ++p) {
        printf("\t%s\n", *p);
    }

    
    return 0;
}
