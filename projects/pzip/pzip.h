#ifndef __PZIP_H__
#define __PZIP_H__

#include <pthread.h>
#include <stdio.h>

// thread functions to wait and signal threads
void thr_wait();
void thr_exit();

// buf_split: split file buffer into threads sections
char **buf_split(char *buf, int size, int threads);

// zip: compress and return a string
char *zip(char *s);

// tzip: send thread's compressed string to stdout
void *tzip(void *arg);

#endif
