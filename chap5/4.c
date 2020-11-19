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
		printf("in child process...\n");
		char *args[] = {"ls", "-l", "-a", NULL};
	    
		// any function above the function to be called needs
		// to be commented out, otherwise only the top function
		// will be called
		execl("/bin/ls", "ls", "-l", "-a", NULL);
		execle("/bin/ls", args[0], "-l", "-a", NULL, NULL);
		execlp(args[0], "ls", "-l", "-a", NULL);
		execv("/bin/ls", args);
		execvp(args[0], args);
		// execvpe() is a GNU extension

	}
	return 0;
}
