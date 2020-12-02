#ifdef Linux
	#define _GNU_SOURCE
#endif
#define _GNU_SOURCE
// default MacOS 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>

#define handle_error_en(en, msg) \
	           do { errno = en; perror(msg); exit(EXIT_FAILURE);  } while (0)

int main(int argc, char **argv)
{

	if (argc != 3) {
		fprintf(stderr, "usage: tlb <pages> <trials>\n");
		exit(1);
	}

	int i, s, j;
	cpu_set_t cpuset;
	pthread_t thread = pthread_self();
	CPU_ZERO(&cpuset);
	for (j = 0; j < 8; j++)
		CPU_SET(j, &cpuset);
	
	// set the program usage to only one cpu
	s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
	if (s != 0)
		handle_error_en(s, "pthread_setaffinity_np");

	//long PAGESIZE = sysconf(_SC_PAGESIZE); // 4096
	long PAGESIZE = 16;

	int pages = atoi(argv[1]);
	int trials = atoi(argv[2]);

	int *a = (int*)malloc(pages * PAGESIZE);
	if (a == NULL) {
		fprintf(stderr, "memory problems...\n");
		exit(1);
	}

	long jump = PAGESIZE/sizeof(int);

	struct timeval start, stop;

	gettimeofday(&start, NULL);	
	
	for (i = 0; i < pages * jump; i += jump)
		a[i] += 1;
	
	gettimeofday(&stop, NULL);	

	long ttime = 1000000 * (stop.tv_sec - start.tv_sec)+(stop.tv_usec - start.tv_usec);

	printf("%ld seconds\n", ttime);
	
	return 0;
}
