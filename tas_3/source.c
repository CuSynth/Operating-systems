#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void openClose(char* fname) {
    FILE* f = fopen(fname, "r");
    if(!f)
        perror("Failed to open file");
    else {
        printf("File opened and closed successfully.\n");
        fclose(f);
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Invalid argument!\n");
        exit(1);
    }

    uid_t rUID = getuid();
    uid_t eUID = geteuid();
    printf("Real UID: %d\nEffective UID: %d\n",
            rUID, eUID);

    printf("Trying to open file..\n");
    openClose(argv[1]);

    printf("Setting eUID as new rUID.\n");
    setuid(eUID);

    rUID = getuid();
    eUID = geteuid();
    printf("Real UID: %d\nEffective UID: %d\n",
            rUID, eUID);

    printf("Trying to open file..\n");
    openClose(argv[1]);

    exit(0);
}
