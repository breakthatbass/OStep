#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLINE 1024

void reverse(char **s, FILE *p)
{
    while(*--s)
        fprintf(p, "%s", *s);
}

int main(int argc, char **argv)
{
    int infile = 0;
    FILE *in;
    FILE *out;
    struct stat z;
    int status, size;
    char **s;

    if (argc < 2) {
        // no args. we read from stdin and print to stdout
        in = stdin;
    } else if (argc == 2) {
        // we read from file and print to stdout
        infile = 1;
        in = fopen(argv[1], "r");
        out = stdout;
        if (in == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    } else if (argc == 3) {
        // have an in and out file
        infile = 1;
        in = fopen(argv[1], "r");
        out = fopen(argv[2], "w");
        if (in == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        } if (out == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    } else {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // get size of file
    if (infile == 1) {
        int fint = open(argv[1], O_RDONLY);
        status = fstat(fint, &z);
	    size = z.st_size;
        close(fint);
        s = malloc(sizeof(char)*size+1);
        if (s == NULL) {
            fprintf(stderr, "malloc falied\n");
            exit(1);
        }
    } else {
        s = malloc(sizeof(char*) * MAXLINE);
        if (s == NULL) {
            fprintf(stderr, "malloc falied\n");
            exit(1);
        }
    }

    char *buf = NULL;
    size_t sz = 0;

    while (getline(&buf, &sz, in) != EOF) {
        *s = malloc(sizeof(char)*MAXLINE);
        if (s == NULL) {
            fprintf(stderr, "malloc falied\n");
            exit(1);
        }
        strcpy(*s, buf);
        s++;
    }
    reverse(s, out);

    fclose(in);
    fclose(out);
    free(s)

    return 0;
}