#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t {
    // add semaphores and other information here
    sem_t *s;
    sem_t *all_in;
    int threads;
    int tin;    //keep track of threads in barrier
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    // initialization code goes here
    b->s = sem_open("/s", 1);
    b->all_in = sem_open("/all_in", 0);
    b->threads = num_threads;
    b->tin = 0;
}

void barrier(barrier_t *b) {
    // barrier code goes here
    sem_wait(b->s);
    sem_wait(b->all_in);
    b->tin++;

    // hold them up here until b-tin == b->threads
    while (b->tin < b->threads)
        ; // simple spinner works...but deadlock can occasionally happen

    sem_post(b->all_in);
    sem_post(b->s);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    sleep(1);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}

// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	pthread_join(p[i], NULL);

    printf("parent: end\n");
    sem_close(b.s);
    sem_close(b.all_in);
    sem_unlink("/s");
    sem_unlink("/all_in");
    return 0;
}