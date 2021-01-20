#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <ctype.h>

// timing
// 1 - 0.12
// 2 - 0.18
// 3 - 0.22
// 4 - 0.26

#define MIL 1000000
//#define THREADS 4

// hand-over-hand locking means each node has its own lock
typedef struct node {
    struct node *next;
    pthread_mutex_t lock;
    long long key;
} node_t;

typedef struct list {
    node_t *head;
} list_t;


static node_t *new_node(long long key)
{
    node_t *node = malloc(sizeof(node_t));
    assert(node);

    pthread_mutex_lock(&node->lock);
    node->key = key;
    node->next = NULL;
    pthread_mutex_unlock(&node->lock);

    return node;
}


// creating an empty node when initiating the list
// is required. otherwise a random segfault
// will occur somewhere between the first to fifth time trying
// to lock the node->lock in the insert function.
// the seg fault comes from EXC_BAD_ACCESS and I can't find
// exactly what is causing it. oh well, this fixes it!
void list_init(list_t *l)
{   
    node_t *node = new_node(0);
    l->head = node;
}


void insert(list_t *l, long long key)
{
    pthread_mutex_lock(&l->head->lock);

    node_t *node = new_node(key);
    //pthread_mutex_lock(&node->lock);
    node->next = l->head;
    l->head = node;
    //pthread_mutex_unlock(&node->lock);
    pthread_mutex_unlock(&l->head->lock);
}


void print_list(list_t *l)
{
    node_t *tmp = l->head;
    pthread_mutex_lock(&tmp->lock);
    while (tmp->next) {
        printf("%lld\n", tmp->key);
        tmp = tmp->next;
    }
    pthread_mutex_unlock(&tmp->lock);
}


void *tfunc(void *arg)
{
    uint64_t i;
    list_t *l = (list_t*)arg; // arg is a list
    for (i = 0; i < MIL; i++)
        insert(l, i);
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

    return 0;
}