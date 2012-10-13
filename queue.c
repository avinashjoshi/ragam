/*
 * File: queue.c
 *
 * This file contains all functions to manupalate queue
 */

#include "my_header.h"

/*
 * Insert element (node) to
 * top of queue
 */
void
insert_queue ( int node, int ts ) {
	queue *p = root;
	queue *temp = malloc ( sizeof(queue) );
	// Creating the temp queue
	temp->node_number = node;
	temp->timestamp = ts;
	temp->next = NULL;
	if ( p == NULL ) {
		root = temp;
		return;
	}
	while ( p->next != NULL ) {
		p = p->next;
	}
	p->next = temp;
}

/*
 * Remove node from top of queue
 * Return type is queue
 */
queue
*remove_queue ( void ) {
	if ( is_queue_empty() )
		return NULL;
	else {
		queue *p = root;
		root = root->next;
		return p;
	}
}

/*
 * Check if queue is empty!
 */
int
is_queue_empty ( void ) {
	if ( root == NULL )
		return 1;
	else
		return 0;
}

/*
 * Dummy print function
 * to check if queue works...
 */
void
print_queue ( void ) {
	if ( is_queue_empty() )
		return;
	queue *p = root;
	while ( p != NULL ) {
		fprintf (stdout, "%d: %d\n", p->node_number, p->timestamp );
		p = p->next;
	}
}
