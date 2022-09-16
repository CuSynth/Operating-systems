#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TZ "TZ=America/Los_Angeles"

extern char *tzname[];
extern time_t timezone;

int main() {
    putenv(TZ);
    
    time_t now;
    now = time(NULL);
    
    struct tm *sp;    
    sp = localtime(&now);

    printf("ctime: %s", ctime(&now));
    printf("full time: %d/%d/%02d %d:%02d %s\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year, sp->tm_hour,
        sp->tm_min, tzname[sp->tm_isdst]);

    exit(0);
}