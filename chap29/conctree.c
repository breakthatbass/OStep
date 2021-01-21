/**
 * this implementation comes from two books:
 * 1. Algorithms by Robert Sedgewick pp. 398 - 399
 * 2. K&R by Kernighan and Ritchie pp. 140 - 142
 *
 * i used some of the functions from k&r but used partially the 
 * tree node structure from algorithms with making a struct that
 * points to 'root' as a way to imlement both versions of
 * a concurrent btree similar to the two versions of the linked
 * list in the chapter.
 *
 * **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <ctype.h>
#include <sys/time.h>
//#include <time.h>

// easier error checking
#define lock_doors(lock) assert(pthread_mutex_lock(lock) == 0);
#define unlock_doors(lock) assert(pthread_mutex_unlock(lock) == 0);

#define MIL 1000000

// structure of a node with a lock
typedef struct node {
	char *key;
	char *value;
	struct node *left;
	struct node *right;
} node_t;

// structure that keep tracks of nodes, and points to root
typedef struct tree {
	node_t *root;
	pthread_mutex_t tree_lock;
	int count;
} tree_t;


// tree_init: initiate a tree and lock
void tree_init(tree_t *t)
{
	t->root = NULL;
	t->count = 0;
	pthread_mutex_init(&t->tree_lock, NULL);
}


// new_node: create and return a new node for the tree
static node_t *new_node(tree_t *t, char *key, char *value)
{
	node_t *n = malloc(sizeof(node_t));
	assert(n);

	lock_doors(&t->tree_lock);
	n->key = strdup(key);
	n->value = strdup(value);
	n->left = NULL;
	n->right = NULL;
	t->count++;
	unlock_doors(&t->tree_lock);

	return n;
}


// add_node: add a node n to the proper place in tree t
void add_node(tree_t *t, char *key, char *value)
{
	node_t *new = new_node(t, key, value);

	// add the node into the tree
	int cond;

	lock_doors(&t->tree_lock);
	node_t *tmp = t->root;
	if (tmp == NULL) {
		// this is the first node in tree
		t->root = new;
		//t->count++;
		unlock_doors(&t->tree_lock);
		return;
	}

	// move through tree and find where node should be
	while (tmp != NULL) {
		if ((cond = strcmp(key, tmp->key)) < 0) {
		// we go left;
			if (tmp->left == NULL) {
				tmp->left = new;
				break;
			}
			tmp = tmp->left;
		} else {
			if (tmp->right == NULL) {
				tmp->right = new;
				break;
			}
			tmp = tmp->right;
		}
	}
		unlock_doors(&t->tree_lock);
}


// treeprint: in order print of tree
void treeprint(node_t *t)
{
	node_t *tmp = t;
	if (t != NULL) {
		treeprint(tmp->left);
		printf("%s: %s\n", tmp->key, tmp->value);
		treeprint(tmp->right);
	}
}


// rand_str: create a random string
char *rand_str()
{
	static char s[6];
	const char *alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	for (int i = 0; i < 5; i++)
		s[i] = alphanum[rand() % (strlen(alphanum) - 1)];

	s[5] = 0;
	return s;
}


void *tfunc(void *arg)
{
    uint64_t i;
    tree_t *b = (tree_t*)arg; // arg is a tree
    for (i = 0; i < MIL; i++) {
		char *s = rand_str();
		// add a rand string to tree with a value of "1"
		add_node(b, s, "1");
	}
    return NULL;
}


int main(int argc, char **argv)
{
	int i, threads;
	pthread_t *t;	// array of threads
	tree_t *b;

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
	b = malloc(sizeof(tree_t)); assert(b); 
	tree_init(b);

	gettimeofday(&start, NULL);	
	for (i = 0; i < threads; i++)
        assert(pthread_create(&t[i], NULL, &tfunc, b) == 0);

    for (i = 0; i < threads; i++)
        assert(pthread_join(t[i], NULL) == 0);
    gettimeofday(&stop, NULL);

    // compute total time
	ttime = (float)(stop.tv_sec * MIL + 
			stop.tv_usec - 
			start.tv_sec * 
			MIL - start.tv_usec)/MIL;

	//ttime = (float)

	//treeprint(b->root);
	printf("%d\n", b->count);
	printf("time: %f\n", ttime);
	

	free(b);
	free(t);
	return 0;
}
