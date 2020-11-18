/**
 * chapter 5: process api
 * exercise 4: write a program that calls fork() and then calls some form of
 * exec() to run the program /bin/ls. see if you can try all the variants of
 * exec(), including (on Linux) execl(), execle(), execlp(), execv(), execvp(),
 * execvpe(). why do you think there are so many variants of the same basic
 * call?
 *
 *  - not exactly sure. my guess is that it can be called on files and shell
 *  commands with the ability to not only give commands with each, but also the
 *  ability to define the environment (which I just have defined as "NULL").
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
