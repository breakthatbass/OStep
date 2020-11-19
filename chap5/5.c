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
	int rc_wait = wait(NULL);
	printf("child process - pid: %d\n", getpid()); 
	printf("wait() returns %d\n", rc_wait);
}
else	// parent process
{
	int rc_wait = wait(NULL);
	printf("parent process - pid: %d\n", getpid());
	printf("wait() returns %d\n", rc_wait);
}


	return 0;
}
