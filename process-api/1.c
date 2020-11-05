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
        x = 101;
        printf("child x: %d\n", x);
    } else {    // parent process
        int rc_wait = wait(NULL);   // wait until child process is done
        printf("parent x: %d\n", x);
        x= 102;
        printf("redefined parent x: %d\n", x);
    }
    return 0;
}