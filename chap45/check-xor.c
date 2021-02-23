#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv)
{
    unsigned char checksum;
    int fd, rc;
    char c;

    assert(argc == 2);

    fd = open(*++argv, O_RDONLY);
    assert(fd > -1);

    while ((rc = read(fd, &c, 1)) != 0) {
        assert(rc > -1);
        if (c != '\n')
            checksum ^= (c - '0'); 
    }

    printf("checksum: %d\n", checksum);
    return 0;
}