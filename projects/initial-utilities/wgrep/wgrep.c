#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 450

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    char *searchterm = argv[1];
    FILE *fp;
    char buf[MAXLINE];
  
    if (argc == 2) {
        // stdin
        while (fgets(buf, MAXLINE, stdin)) {
            if (strstr(buf, searchterm) == NULL) continue;
            else printf("%s", buf);
            memset(buf, 0, MAXLINE);
        }
    }
    else {
        while (--argc > 1) {
            fp = fopen(argv[argc], "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }
            while (fgets(buf, MAXLINE, fp)) {
                if (strstr(buf, searchterm) == NULL) continue;
                else printf("%s", buf);
                memset(buf, 0, MAXLINE);
            }
        }
    }
    return 0;
}