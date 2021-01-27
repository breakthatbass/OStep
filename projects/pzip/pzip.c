#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pzip.h"

static int done = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;


// thr_wait: stop and wait for thread to finish before moving on
void pthread_wait()
{
    _pthread_mutex_lock(&m);
    while (done == 0)
        _pthread_cond_wait(&c, &m);
    _pthread_mutex_unlock(&m);
}


// thr_exit: send signal that thread is done
void pthread_done()
{
    _pthread_mutex_lock(&m);
    done = 1;
    _pthread_cond_signal(&c);
    _pthread_mutex_unlock(&m);
}


// buf_split: split file buffer into threads sections
char **buf_split(char *buf, int size, int threads)
{
    char **split_buf;
    int portion, extra;
    int n = threads;

    portion = size/threads;
    extra = size%threads;
    extra+=portion;

    split_buf = malloc(sizeof(char*)*size);
    assert(split_buf);

    // if the portion division leaves an offset, add that
    // extra amount to the first string
    *split_buf = malloc(sizeof(char)*extra);
    assert(*split_buf);
    memcpy(*split_buf, buf, extra);
    buf += extra;
    split_buf++;

    while (--threads > 0) {
        *split_buf = malloc(sizeof(char)*portion);
        assert(*split_buf);
        memcpy(*split_buf, buf, portion);
        buf += portion;
        split_buf++;
    }
    split_buf -= n;
    return split_buf;
}


// zip: compress and return a string
char *zip(char *s)
{    
    char *zs = malloc(sizeof(char)*strlen(s)+1);
    assert(zs);
    int prev, count, i;
    count = 1;
    i = 0;

    while (*s) {
        if (count == 9) { // prevent double digits
            *zs++ = count+'0';
            *zs++ = prev;
            i+=2;
            count = 1;
        } else if (prev == *s) count++;
        else if (*s != prev && count > 1) {
            *zs++ = count+'0';
            *zs++ = prev;
            i+=2;
            count = 1;
        } else {
            *zs++ = prev;
            i++;
        }
        prev = *s;
        s++;
    }
    *zs = prev; // last char
    // bring pointer back
    zs -= i-1;
    return zs;
}


// tzip: take thread's string and print it to stdout
void *tzip(void *arg)
{
    char *str = (char*)arg;
    _pthread_mutex_lock(&m);
    char *new_zip = zip(str);
    fprintf(stdout, "%s\n", new_zip);
    _pthread_mutex_unlock(&m);
    pthread_done();
    return NULL;
}
