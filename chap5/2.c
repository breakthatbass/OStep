#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
	int fd;  // file descriptor, for open 
	int fw;  // var used to write to fd
	int rc;  // var used to fork processes

	fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC , S_IRWXU);
	assert(fd > -1);

	rc = fork();
	if (rc < 0) 
	{   // fork failed, exit
		fprintf(stderr, "fork failed\n");
		exit(1);
	} 
	else if (rc == 0) 
	{   // child process
		fw = write(fd, "hello from the child process\n", 
				strlen("hello from the child process\n")); 
	} 
	else 
	{   // parent process (main)
		fw = write(fd, "hello from the parent process\n", 
				strlen("hello from the parent process\n")); 
	}

	close(fd);
	return 0;
}
