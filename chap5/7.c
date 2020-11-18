/**
 * chapter 5: process api
 * exercise 7: write a program that creates a child process, and then in the
 * child closes standard output (STDOUT_FILENO). what happens if the child calls
 * printf() to print some output after closding the descriptor.
 *  - it doesn't print anything.
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
		close(STDOUT_FILENO);
		printf("hello\n");
	}

	return 0;
}
