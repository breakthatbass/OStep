/**
 * chapter 5: prcoess api
 * exercise 5: now write a program that uses wait() to wait for the child
 * process to finish in the parent. 
 *
 * what does wait() return? 
 * - it returns the pid of the child process
 *
 * what happens if you use wait() in the child?
 *  - it returns -1 which means that process has no child processes.
 *  so nothing really happens. it just moves on. 
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
