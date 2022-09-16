#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#define BUFF_SZ 128

int main(int argc, char *argv[]) {
    char buff[BUFF_SZ];

    int size;
    while((size = read(STDIN_FILENO, buff, BUFF_SZ))) {
        for (int i = 0; i < size; i++)
            buff[i] = toupper(buff[i]);
        write(STDOUT_FILENO, buff, size);
    }
    write(STDOUT_FILENO, "\n", 1);

    return 0;
}