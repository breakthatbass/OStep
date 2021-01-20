#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

// one thread: 3 secs
// two threads: 7 secs
// three threads: 8 secs
// four threads: 9 secs

#define MIL 1000000

#define lock_doors(lock) assert(pthread_mutex_lock(lock) == 0);
#define unlock_doors(lock) assert(pthread_mutex_unlock(lock) == 0);

typedef struct counter {
    int value;
    pthread_mutex_t lock;
} counter_t;


void init(counter_t *c)
{
    c->value = 0;           
    assert(pthread_mutex_init(&c->lock, NULL) == 0);
}

void increment(counter_t *c)
{
    lock_doors(&c->lock);
    c->value++;
    unlock_doors(&c->lock);
}

void decrement(counter_t *c)
{
    lock_doors(&c->lock);
    c->value--;
    unlock_doors(&c->lock);
}

int get(counter_t *c)
{
    lock_doors(&c->lock);
    int rc = c->value;
    unlock_doors(&c->lock);
    return rc;
}

void *update(void *arg)
{
    for (int i = 0; i < MIL; i++)
        increment((counter_t*)arg);
    return NULL;
}

int main()
{
    counter_t *c;
    init(c);

    pthread_t p1, p2, p3, p4;
    int ec;     // error checking variable

    float ttime;
    struct timeval start, stop;


    gettimeofday(&start, NULL);		// get time now, store in start

    pthread_create(&p1, NULL, update, c);
    pthread_create(&p2, NULL, update, c);
    pthread_create(&p3, NULL, update, c);
    pthread_create(&p4, NULL, update, c);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);

    gettimeofday(&stop, NULL);		// get time now, store in stop

    // compute total time
	ttime = (float)(stop.tv_sec * MIL + 
			stop.tv_usec - 
			start.tv_sec * 
			MIL - start.tv_usec)/MIL;


    printf("count: %d\n", get(c));
	printf("time: %f\n", ttime);

    return 0;
}