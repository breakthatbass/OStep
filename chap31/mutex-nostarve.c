#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

/* 
* i'm pretty much pulling all of this from Downey's
* Little Book of Semaphores as suggested in the OStep text.
* In chapter 4, Downey pretty much answers this problem
*/

typedef struct __ns_mutex_t {
    int room1;      // room 1 & 2 keeps track of # of threads in waiting room
    int room2;
    sem_t *mutex;   // helps protect the counters
    sem_t *t1;      // turnstiles
    sem_t *t2;
} ns_mutex_t;

char *mutexstr = "/mutex";
char *t1str = "/t1";
char *t2str = "/t2";

void ns_mutex_init(ns_mutex_t *m) {
    // MacOS requirements...
    /*
    Sem_unlink(mutexstr);
    Sem_unlink(t1str);
    Sem_unlink(t2str);*/

    m->room1 = 0;
    m->room2 = 0;
    m->mutex = Sem_open(mutexstr, 1);
    m->t1 = Sem_open(t1str, 1);
    m->t2 = Sem_open(t2str, 0);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Sem_wait(m->mutex);
    m->room1++;
    Sem_post(m->mutex);

    Sem_wait(m->t1);
    m->room2++;
    Sem_wait(m->mutex);
    m->room1--;

    if (m->room1 == 0) {
        Sem_post(m->mutex);
        Sem_post(m->t2);
    } else {
        Sem_post(m->mutex);
        Sem_post(m->t1);
    }

    Sem_wait(m->t1);
    m->room2--;    
}

void ns_mutex_release(ns_mutex_t *m) {
    if (m->room2 == 0) {
        Sem_post(m->t1);
    } else {
        Sem_post(m->t2);
    }
}

ns_mutex_t m;
int j = 0;

void *worker(void *arg) {
    ns_mutex_acquire(&m);
    j++;
    ns_mutex_release(&m);
    return NULL;
}

int main(int argc, char *argv[]) {
    
    assert(argc == 2);
    int threads = atoi(*++argv);

    int i;
    ns_mutex_init(&m);
    pthread_t thr[threads];
    printf("parent: begin\n");

    for (i = 0; i < threads; i++)
	    Pthread_create(&thr[i], NULL, worker, NULL);

    for (i = 0; i < threads; i++)
	    Pthread_join(thr[i], NULL);

    printf("count: %d\n", j);
    printf("parent: end\n");

    Sem_close(m.mutex);
    Sem_close(m.t1);
    Sem_close(m.t2);

    Sem_unlink(mutexstr);
    Sem_unlink(t1str);
    Sem_unlink(t2str);
    return 0;
}