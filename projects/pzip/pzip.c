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

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

// buf_split: split file buffer into threads sections
char **buf_split(const char *buf, int size, int threads)
{
    char **split_buf;
    int portion, offset;

    portion = size/threads;
    offset = size-(portion*threads);
    offset+=portion;

    split_buf = malloc(sizeof(char)*size+threads);
    assert(split_buf != NULL);

    // if the portion division leaves an offset, add that
    // extra amount to the first string
    if (offset > 0) {
        *split_buf = malloc(sizeof(char)*offset+1);
        assert(*split_buf != NULL);
        memcpy(*split_buf, buf, offset);
        buf += offset;
        split_buf++;
    }

    while (*buf) {
        *split_buf = malloc(sizeof(char)*portion+1);
        assert(*split_buf != NULL);
        memcpy(*split_buf, buf, portion);
        buf += portion;
        split_buf++;
    }
    split_buf -= threads;
    return split_buf;
}


// zip: compress and return a string
char *zip(char *s)
{
    char *zs = malloc(sizeof(char)*strlen(s));
    assert(zs);
    int prev, count, c, i;
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
    //printf("%s\n", zs);
    return zs;
}


FILE *zip_write(char *s, char *f)
{
    FILE *fp = fopen(f, "a");
    fprintf(fp, "%s", s);
    return fp;
}


void *tzip(void *arg)
{
    char *str = (char*)arg;
    assert(pthread_mutex_lock(&m) == 0);
    char *new_zip = zip(str);
    FILE *fp = zip_write(new_zip, "com.txt");
    fclose(fp);
    assert(pthread_mutex_unlock(&m) == 0);
    return NULL;
}


int main(int argc, char **argv)
{
    struct stat s;
    pthread_t *t;   // array of threads
    char *buffer;
    const char *file;
    int threads, fd, status, size, rdr, i;
    float ttime;	
    clock_t start = clock();

    //file = "../../LICENSE";
    file = "test.txt";
    //file = "../../../crap/bible.txt";

    // get # of cpus on system
    threads = sysconf(_SC_NPROCESSORS_ONLN);

    // open file
	fd = open(file, O_RDONLY);
    assert(fd > -1);

    // get size of file
	status = fstat(fd, &s);
	size = s.st_size;

    buffer = malloc(sizeof(char)*size+2); assert(buffer);
	
    // read into file into buffer
    rdr = read(fd, buffer, size);
    assert(rdr > 0);

    // split file into equal chunks
    char **test = buf_split(buffer, size, threads);

    t = malloc(sizeof(pthread_t)*threads); assert(t);
    for (i = 0; i < threads; i++) {
        assert(pthread_create(&t[i], NULL, tzip, test[i]) == 0);
        // join in the same loop to resconstruct
        // the new file the same as the old
        assert(pthread_join(t[i], NULL) == 0);
    }
   /*******************************************
    for (i = 0; i < threads; i++)
       assert(pthread_join(t[i], NULL) == 0);
**********************************************/
    clock_t end = clock();
    ttime = (end - start)/(double)CLOCKS_PER_SEC;
    printf("time: %f\n", ttime);

    free(test);
    free(buffer);

    return 0;
}