#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <ctype.h>

#define NUMCPUS 4
#define MIL 1000000

typedef struct counter {
    int global;
    pthread_mutex_t glock;      // global lock
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS]; // locks for each local counter
    int threshold;
} counter_t;


// init: record threshold, init locks, init values of 
// all local counts and global count
void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;

    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}


void update(counter_t *c, int threadID, int amt)
{
    int cpu = threadID % NUMCPUS;

    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        // transfer to global
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}


// get: return global amount
int get(counter_t *c)
{
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val; // only approximate
}

// function pointer for threads
static void *tfunc(void *arg)
{
    pthread_t threadID = pthread_self();  // nifty function
    for (int i = 0; i < MIL; i++)
        update((counter_t*)arg, (int)threadID, 1); // just add 1 to counter
    return NULL;
}


int main(int argc, char **argv)
{
    int thold, i;
    counter_t *c;   // our counter struct
    pthread_t *t;   // array of threads (num of cpus)
    // timing stuff
    float ttime;
    struct timeval start, stop;

    // make sure usage is right
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out <threshold>\n");
        exit(EXIT_FAILURE);
    }
    if (!isdigit(*argv[1])) {
        fprintf(stderr, "threshold must be a number\n");
        exit(EXIT_FAILURE);
    }
    thold = atoi(argv[1]);

    c = malloc(sizeof(counter_t)); assert(c);
    t = malloc(sizeof(pthread_t)*NUMCPUS); assert(t);

    init(c, thold);

    gettimeofday(&start, NULL);	
    // no error checking as to not spend cpu power/time on error checking
    // to hopefully have more accurate timing
    for (i = 0; i < NUMCPUS; i++)
        pthread_create(&t[i], NULL, tfunc, c);

    for (i = 0; i < NUMCPUS; i++)
        pthread_join(t[i], NULL);
    gettimeofday(&stop, NULL);

    // compute total time
	ttime = (float)(stop.tv_sec * MIL + 
			stop.tv_usec - 
			start.tv_sec * 
			MIL - start.tv_usec)/MIL;

    printf("count: %d\n", get(c));
	printf("time: %f\n", ttime);

    free(c);
    free(t);

    return 0;
}
