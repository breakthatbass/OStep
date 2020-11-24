#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int mb, len, runtime, i;
	int *array;
	clock_t t;
	double t_spent;

	if (argc < 2 || argc > 3) {
		fprintf(stderr, "usage: memusr: <MB> <time>\n");
		exit(1);
	}
	
	printf("pid: %d\n", getpid());
	
	if ((atoi(argv[1]) > 2000000)) {  
	   	// if above 2 million, malloc fails
		fprintf(stderr, "max MB: 2000000\n");
		exit(1);
	}
	
	// megabytes to bytes for malloc
	mb = atoi(argv[1]) * 1024;
	runtime = atoi(argv[2]);

	len = mb/sizeof(int);
	array = malloc(mb);
	
	t = clock();	// start timer
	while (1) {
		t_spent = (double)(clock() - t)/CLOCKS_PER_SEC;
		if (t_spent >= runtime)
			break;
		for (i = 0; i < len; i++)
			array[i] += 1;	
	}

	free(array);
	return 0;
}
