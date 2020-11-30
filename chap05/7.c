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
