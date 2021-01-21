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
		//t->count++;
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


// tree_init: initiate a tree and lock
void tree_init(tree_t *t)
{
	t->root = NULL;
	t->count = 0;
	assert(pthread_mutex_init(&t->tree_lock, NULL) == 0);
}


int main(int argc, char **argv)
{
	// assume int for now
	//int threads = atoi(*++argv);

	tree_t *b = malloc(sizeof(tree_t)); assert(b); 
	tree_init(b);

	add_node(b, "taylor", "33");
	add_node(b, "catharine", "37");
	add_node(b, "suki", "5");

	printf("%d\n", b->count);

	treeprint(b->root);

	free(b);
	return 0;
}
