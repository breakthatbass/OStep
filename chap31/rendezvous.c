#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

sem_t *s1, *s2;

void *child_1(void *arg) {
    printf("child 1: before\n");
    // what goes here?
    sleep(1);
    sem_wait(s1);
    sem_wait(s2);
    printf("child 1: after\n");
    sem_post(s1);
    sem_post(s2);
    return NULL;
}

void *child_2(void *arg) {
    printf("child 2: before\n");
    // what goes here?
    sleep(1);
    sem_wait(s1);
    sem_wait(s2);
    printf("child 2: after\n");
    sem_post(s1);
    sem_post(s2);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    printf("parent: begin\n");
    // init semaphores here
    s1 = sem_open("/sem1", 0);
    s2 = sem_open("/sem2", 0);
    pthread_create(&p1, NULL, child_1, NULL);
    pthread_create(&p2, NULL, child_2, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    sem_close(s1);
    sem_close(s2);
    sem_unlink("/sem1");
    sem_unlink("/sem2");
    printf("parent: end\n");
    return 0;
}