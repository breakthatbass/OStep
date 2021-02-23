// fletcher algorithm
// 
// two checksum bytes, s1, s2
// block D (file) consists of bytes d1...dn
// s1 = (s1 + di) mod 255
// s2 = (s2 + s1) mod 255

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv)
{
    int fd, rc, s1, s2;
    char c;

    s1 = 0;
    s2 = 0;

    assert(argc == 2);

    fd = open(*++argv, O_RDONLY);
    assert(fd > -1);

    while ((rc = read(fd, &c, 1)) != 0) {
        assert(rc > -1);
        if (c != '\n')
            s1 = ((c - '0') + s1) % 255;
            s2 = (s2 + s1) % 255;
    }

    printf("s1: %d s2: %d\n", s1, s2);
    close(fd);
    return 0;
}