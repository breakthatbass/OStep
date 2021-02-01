#ifndef __sem_h__
#define __sem_h__

#include <pthread.h>
#include <assert.h>

// macros for error checking
#define cond_init(cond)         assert(pthread_cond_init(cond) == 0);
#define mutex_init(lock)        assert(pthread_mutex_init(lock) == 0);
#define mutex_lock(lock)        assert(pthread_mutex_lock(lock) == 0);
#define mutex_unlock(lock)      assert(pthread_mutex_lock(lock) == 0);
#define cond_wait(cond, lock)   assert(pthread_cond_wait(cond, lock) == 0);
#define cond_signal(cond)       assert(pthread_cond_signal(cond) == 0);

typedef struct zem_t {
    int value;
    pthread_cond_t cond;
    pthread_mutex_t lock;
} zem_t;

void zem_init(zem_t *s, int value);
void zem_wait(zem_t *s);
void zem_post(zem_t *s);

#endif