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
	pthread_mutex_t node_lock;
} node_t;

// structure that keep tracks of nodes, and points to root
typedef struct tree {
	node_t *root;
	int count;
} tree_t;

// new_node: create and return a new node for the tree
static node_t *new_node(tree_t *t, char *key, char *value)
{
	node_t *n = malloc(sizeof(node_t));
	assert(n);
	
	assert(pthread_mutex_init(&n->node_lock, NULL) == 0);

	lock_doors(&n->node_lock);
	n->key = strdup(key);
	n->value = strdup(value);
	n->left = NULL;
	n->right = NULL;
	t->count++;
	unlock_doors(&n->node_lock);

	return n;
}

// add_node: add a node n to the proper place in tree t
void add_node(tree_t *t, char *key, char *value)
{
	int cond;

	// check first if tree is empty
	if (t->root == NULL) {
		// this is the first node in tree
		node_t *new = new_node(t, key, value);
		t->root = new;
		return;
	}
	
	// otherwise lock it up and add a new node
	lock_doors(&t->root->node_lock);
	node_t *new = new_node(t, key, value);
	node_t *tmp = t->root;
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
	unlock_doors(&t->root->node_lock);
}


// tree_init: initiate a tree and lock
void tree_init(tree_t *t)
{
	t->root = NULL;
	t->count = 0;
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
	char *s;

    tree_t *b = (tree_t*)arg; // arg is a tree

    for (i = 0; i < MIL; i++) {
		s = rand_str();
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
        assert(pthread_create(&t[i], NULL, tfunc, b) == 0);

	//char *m;
    for (i = 0; i < threads; i++)
        assert(pthread_join(t[i], NULL) == 0);
	
	//free(b);
    printf("here\n");
	gettimeofday(&stop, NULL);
	

    // compute total time
	ttime = (float)(stop.tv_sec * MIL + 
			stop.tv_usec - 
			start.tv_sec * 
			MIL - start.tv_usec)/MIL;


	//treeprint(b->root);
	printf("%d\n", b->count);
	printf("time: %f\n", ttime);
	

	free(b);
	free(t);
	return 0;
}
