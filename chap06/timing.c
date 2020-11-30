#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#define loops 100000


int main(int argc, char **argv)
{
	int fd;
	float ttime;

	struct timeval start, stop;

	fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	assert(fd > -1);
	
	gettimeofday(&start, NULL);		// get time now, store in start
	
	for (int i = 0; i < loops; i++)
		read(fd, NULL, 0);

	gettimeofday(&stop, NULL);		// get time now, store in stop

	// compute total time
	ttime = (float)(stop.tv_sec * loops + 
			stop.tv_usec - 
			start.tv_sec * 
			loops - start.tv_usec)/loops;

	printf("%f seconds\n", ttime);
	close(fd);
	// rm test file created
	system("rm test.txt");

	return 0;
}
