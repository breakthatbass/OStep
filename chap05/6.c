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

