#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#define loops 10000000	// ten million


// read_loop: do an empty read() system call loops times
void read_loop(int fd)
{
	char to;
	int i;

	for (i = 0; i < loops; i++)
		read(fd, &to, 0);
}



int main(int argc, char **argv)
{
	int fd;
	float ttime;

	struct timeval start, stop;

	fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	assert(fd > -1);
	
	gettimeofday(&start, NULL);		// get time now, store in start
	read_loop(fd);					// read from fd loops times
	gettimeofday(&stop, NULL);		// get time now, store in stop

	// compute total time
	ttime = (float)(stop.tv_sec * loops + 
			stop.tv_usec - 
			start.tv_sec * 
			loops - start.tv_usec)/loops;

	printf("%f\n", ttime);
	close(fd);
	// rm test file created
	system("rm test.txt");

	return 0;
}
