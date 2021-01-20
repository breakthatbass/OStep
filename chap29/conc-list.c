#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <ctype.h>

// 1 - 0.07
// 2 - 0.12
// 3 - 0.15
// 4 - 0.18

#define MIL 1000000
#define THREADS 4

// basic node structure
typedef struct __node_t {
	uint64_t key;
	struct __node_t *next;
} node_t;

// basic list structure (one used per list)
typedef struct __list_t {
	node_t *head;
	pthread_mutex_t lock;
} list_t;


// initiate a list and lock
void list_init(list_t *ll)
{
	ll->head = NULL;
	pthread_mutex_init(&ll->lock, NULL);
}


void list_insert(list_t *ll, uint64_t key)
{
	node_t *new = malloc(sizeof(node_t));
	if (new == NULL) {
		perror("malloc");
		return;
	}
	new->key = key;

	pthread_mutex_lock(&ll->lock);
	new->next = ll->head;
	ll->head = new;
	pthread_mutex_unlock(&ll->lock);
}


int list_lookup(list_t *ll, uint64_t key)
{
	int rv = -1;
	pthread_mutex_lock(&ll->lock);
	node_t *cur = ll->head;
	while(cur) {
		if (cur->key == key) {
			rv = 0;
			break;
		}
		cur = cur->next;
	}
	pthread_mutex_unlock(&ll->lock);
	return rv;	// not found
}


void *tfunc(void *arg)
{
    uint64_t i;
    list_t *l = (list_t*)arg; // arg is a list
    for (i = 0; i < MIL; i++)
        list_insert(l, i);
    return NULL;
}

int main(int argc, char **argv)
{
    int i, threads;
    pthread_t *t;
    list_t *list;

    // timing stuff
    float ttime;
    struct timeval start, stop;

    // make sure usage is right
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out <no. of threads>\n");
        exit(EXIT_FAILURE);
    }
    if (!isdigit(*argv[1])) {
        fprintf(stderr, "threads must be a number\n");
        exit(EXIT_FAILURE);
    }
    threads = atoi(argv[1]);

    t = malloc(sizeof(pthread_t)*threads); assert(t);
    list = malloc(sizeof(list_t)); assert(list);
    
    list_init(list);
    
    gettimeofday(&start, NULL);	
    for (i = 0; i < threads; i++)
        assert(pthread_create(&t[i], NULL, &tfunc, &list) == 0);

    for (i = 0; i < threads; i++)
        assert(pthread_join(t[i], NULL) == 0);
    gettimeofday(&stop, NULL);

    // compute total time
	ttime = (float)(stop.tv_sec * MIL + 
			stop.tv_usec - 
			start.tv_sec * 
			MIL - start.tv_usec)/MIL;

    //print_list(list);

	printf("time: %f\n", ttime);

    free(t);
    free(list);
}