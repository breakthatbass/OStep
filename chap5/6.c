/**
 * chapter 5: process api
 * exercise 6: write a slight modification to the previous program, this time
 * using waitpid() instead of wait(). 
 *
 * when would waitpid() be useful?
 * - this would be helpful to use when there are a lot of processes or if
 *   processes are getting paused or exiting since you can add options to
 *   dictate how wait acts.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
int rc = fork();

if (rc < -1) 
{
	fprintf(stderr, "fork failed\n");
	exit(1);
}
else if (rc == 0)	// child process
{
	printf("child process - pid: %d\n", getpid()); 
}
else	// parent process
{
	int rc_wait = waitpid(-1, NULL, WUNTRACED | WCONTINUED);
	printf("parent process - pid: %d\n", getpid());
}


	return 0;
}

