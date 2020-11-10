/**
 * chapoter 5: process API
 * exercise 1: write a program that calls fork(). before calling fork(), have
 * the main process access a variable (e.g., x) and set its value to something
 * (e.g., 100). 
 *
 * what value is the variable in the child process? 
 * - the value in the child process is the same as the main process unless
 *   redefined in the child process. 
 *
 *
 * what happens to the variable when both the parent and the child change the
 * value of x? 
 * - the changes are only reflected within each of the processes and have no
 *   effect on the other processes.
 *
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int x = 100;
    int rc = fork();
    if (rc < 0) {   // fork failed, exit
        fprintf(stderr, "fork failed...\n");
        exit(1);
    } else if (rc == 0) {   // child process
		printf("child x (unchanged from parent): %d\n", x);
        x = 101;
        printf("child x (changed to 101): %d\n", x);
    } else {    // parent process
        int rc_wait = wait(NULL);   // wait until child process is done
        printf("parent x: %d\n", x);
        x= 102;
        printf("redefined parent x: %d\n", x);
    }
    return 0;
}
