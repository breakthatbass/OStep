#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int pfd[2]; 
	char buf;
	char *msg = "hello from child process 1";

	if (pipe(pfd) < 0)
	{
		perror("pipe failure\n");
		exit(1);
	}

	int rc1 = fork();
	if (rc1 < -1)
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc1 == 0)	// child process 1
	{
		close(pfd[0]);
		// write into the pipe
		write(pfd[1], msg, strlen(msg));
		wait(NULL);
	}
	else	// parent process
	{
		int rc2 = fork(); // child 2
		if (rc2 < -1)
		{
			fprintf(stderr, "fork failed\n");
		}
		else if (rc2 == 0)	// child process 2
		{
			close(pfd[1]);
			// read from the pipe one byte at a time
			while (read(pfd[0], &buf, 1) > 0)
			{
				// write to stdout as it reads from the pipe
				write(STDOUT_FILENO, &buf, 1);
			}
			write(STDOUT_FILENO, "\n", 1);
			close(pfd[0]);
		}
	}
	return 0;
}
