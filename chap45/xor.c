#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    unsigned char checksum;
    int fd, rc;
    char c;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", *argv);
        return 1;
    }

    fd = open(*++argv, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 2;
    }

    while ((rc = read(fd, &c, 1)) != 0) {
        if (rc < 0) {
            perror("read");
            return 3;
        }
        if (c != '\n')
            checksum ^= (c - '0'); 
    }

    printf("checksum: %d\n", checksum);
    return 0;
}