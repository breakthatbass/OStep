#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define MAXWORD 25

int main(int argc, char **argv)
{
    // probably should print an error message
    // but directions want it just to exit returning 0
    if (argc < 2) exit(0);
    
    FILE *fp;
    char buf[MAXLINE];

    while (--argc > 0 && (*argv)) {
        fp = fopen(*++argv, "r");
        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }
        while (fgets(buf, MAXLINE, fp)) {
            sleep(1);
            printf("%s", buf);
            memset(buf, 0, MAXLINE);
        }
    }
    return 0;
}