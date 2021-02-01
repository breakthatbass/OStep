#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t {
    sem_t *lock;
    sem_t *writelock;
    sem_t *readlock;
    int readers;
} rwlock_t;

char *lockstr = "/lock";
char *writelockstr = "/writelock";
char *readlockstr = "/readlock";

void rwlock_init(rwlock_t *rw) {
    // MacOS is requiring unlinking before opening for some reason
    // otherwise it returns SEM_FAILED with errno EACCES - no persmission!
    // this only seems to be required when a variable is passed in for the 
    // first parameter of sem_open rather than a raw string
    sem_unlink(lockstr);
    sem_unlink(writelockstr);
    sem_unlink(readlockstr);

    rw->lock = Sem_open(lockstr, 1);
    rw->writelock = Sem_open(writelockstr, 1);
    rw->readlock = Sem_open(readlockstr, 1);
    rw->readers = 0;
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    Sem_wait(rw->readlock);
    Sem_wait(rw->lock);
    rw->readers++;
    if (rw->readers == 1)
        Sem_wait(rw->writelock);
    Sem_post(rw->lock);
    Sem_post(rw->readlock);
}

void rwlock_release_readlock(rwlock_t *rw) {
    Sem_wait(rw->lock);
    rw->readers--;
    if (rw->readers == 0)
        Sem_post(rw->writelock);
    Sem_post(rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    Sem_wait(rw->readlock);
    Sem_wait(rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
    Sem_post(rw->writelock);
    Sem_post(rw->readlock);
}

//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}