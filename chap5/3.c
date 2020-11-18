/**
 * chapter 5: process api
 * exercise 3: write another program using fork(). the child process should
 * print "hello"; the parent process should print "goodbye". you should ensure
 * that the child process always prints first; can you do this without calling
 * wait() in the parent?
 *
 *  - it is possible. the kill() function can be used to send signals to
 *  specific processes even from other processes based on PIDs.
 *  
 *   - below i used the kill() function in the parent process right before the
 *   "goodbye" prints. Once paused, the child process continues and prints the
 *   "hello" string which then it encounters another kill() call which sends a
 *   signal to the parent process to continue. 
 *
 * */

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
