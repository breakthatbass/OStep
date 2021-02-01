/******************************************************************************
*
*   MacOS requires named semaphores as opposed to unamed semaphores on Linux
*   
*   Initialization looks a bit different:
*   
*   sem_t *s;   // pointers are required
*   s = sem_open("/ostep_semaphore", 0);
*       // sem_post() and sem_wait() are the same
*   sem_close(s);
*   sem_unlink("/ostep_semaphore");
*
*   it has to be closed and unlinked at the end
*   more info: http://blog.cs4u.us/2014/04/using-semaphores-on-mac-os-x.html
*
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

sem_t *s; 

void *child(void *arg) {
    printf("child\n");
    sleep(1);
    // use semaphore here
    sem_post(s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    // init semaphore here
    s = sem_open("/ostep_semaphore", 0);
    pthread_create(&p, NULL, child, NULL);
    // use semaphore here
    sem_wait(s);
    sem_close(s);
    sem_unlink("/ostep_semaphore");
    printf("parent: end\n");
    return 0;
}