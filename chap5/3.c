#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	int rc = fork();

	if (rc < -1) 
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	} 
	else if (rc == 0) 
	{   // child process
		printf("hello\n");
		kill(getpid()-1, SIGCONT);  // continue parent process
	} 
	else 
	{   // parent process
		kill(getpid(), SIGTSTP);  // pause parent process
		printf("goodbye\n");
	}
	return 0;
}
