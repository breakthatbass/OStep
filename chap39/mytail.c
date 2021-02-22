#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void read_file(char *file, int n)
{
    int fd, sz;
    struct stat s;
    char c;

    if (stat(file, &s) < 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    sz = (int)s.st_size;

    char buf[sz];

    fd = open(file, O_RDONLY);
    if (!fd) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, 0, SEEK_END) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    while (n > 0) {
        if (read(fd, &c, 1) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (c == '\n') n--;
        if (lseek(fd, -2, SEEK_CUR) == -1) {
            perror("lseek2:");
            exit(EXIT_FAILURE);
        }
    }

    memset(buf, 0, sz);
    if (read(fd, buf, sz) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
    close(fd);
}

void usage(char *prog) {
    fprintf(stderr, "usage: %s [lines] <file>\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc == 1 || argc > 3)
        usage(argv[0]);
    else if (argc == 2)
        // we can assume the second arg is a file
        read_file(argv[1], 10); // 10 lines by default
    else read_file(argv[2], atoi(argv[1]));
    exit(EXIT_SUCCESS);
}
