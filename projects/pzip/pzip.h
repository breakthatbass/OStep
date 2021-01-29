#ifndef __PZIP_H__
#define __PZIP_H__

#include <pthread.h>
#include <assert.h>

// macros for error checking
#define _pthread_mutex_lock(lock)       assert(pthread_mutex_lock(lock) == 0);
#define _pthread_mutex_unlock(lock)     assert(pthread_mutex_unlock(lock) == 0);
#define _pthread_cond_wait(cond, lock)  assert(pthread_cond_wait(cond, lock) == 0);
#define _pthread_cond_signal(cond)      assert(pthread_cond_signal(cond) == 0);

// thread functions to wait and signal threads
void pthread_wait();
void pthread_exit();

// buf_split: split file buffer into threads sections
char **buf_split(char *buf, int size, int threads);

// zip: compress and return a string
char *zip(char *s);

// tzip: send thread's compressed string to stdout
void *tzip(void *arg);

void free_buf(char **buf, int len);

#endif
