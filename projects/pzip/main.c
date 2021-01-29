#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "pzip.h"


int main(int argc, char **argv)
{
    struct stat s;
    pthread_t *t;   // array of threads
    char *buffer;
    int threads, fd, status, size, rdr, i;

    if (argc == 1) {
        printf("pzip: file1 [file2 ...]\n");
        exit(1);
    }

    // get # of cpus on system
    threads = sysconf(_SC_NPROCESSORS_ONLN);

    while (--argc > 0 && (*argv)) {
        fd = open(*++argv, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "pzip: could not open %s\n", *argv);
            exit(1);
        }

        // get size of file
	    status = fstat(fd, &s);
	    size = s.st_size;

        buffer = malloc(sizeof(char)*size+2); 
        assert(buffer);

        // read into file into buffer
        rdr = read(fd, buffer, size);
        assert(rdr > 0);

        // split file into equal chunks
        char **split_file = buf_split(buffer, size, threads);

        t = malloc(sizeof(pthread_t)*threads);
        assert(t);
        
        for (i = 0; i < threads; i++) {
            assert(pthread_create(&t[i], NULL, tzip, split_file[i]) == 0);
            pthread_wait();  // wait for thread to finish before moving on
        }
        free(split_file);
        free(buffer);
        free(t);
    }

    return 0;
}
