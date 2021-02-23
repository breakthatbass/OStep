#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>

#define MIL 1000000

int main(int argc, char **argv)
{
    unsigned char checksum;
    int fd, rc;
    char c;

    // timing stuff
    float ttime;
    struct timeval start, stop;

    assert(argc == 2);

    fd = open(*++argv, O_RDONLY);
    assert(fd > -1);

    gettimeofday(&start, NULL);	
    while ((rc = read(fd, &c, 1)) != 0) {
        assert(rc > -1);
        if (c != '\n')
            checksum ^= (c - '0'); 
    }
    gettimeofday(&stop, NULL);
    // compute total time
	ttime = (float)(stop.tv_sec * MIL + 
			stop.tv_usec - 
			start.tv_sec * 
			MIL - start.tv_usec)/MIL;

    printf("time: %f\n", ttime);
    printf("checksum: %d\n", checksum);
    close(fd);
    return 0;
}