/*
 * File: d_queue.c
 *
 * This file contains all functions to manupalate d_queue
 */

#include "my_header.h"

/*
 * Insert element (node) to
 * top of d_queue
 */
void
insert_d_queue ( int node, int ts ) {
	d_queue *p = d_q;
	d_queue *temp = malloc ( sizeof(d_queue) );
	// Creating the temp d_queue
	temp->node_number = node;
	temp->timestamp = ts;
	temp->next = NULL;
	if ( p == NULL ) {
		d_q = temp;
		return;
	}
	while ( p->next != NULL ) {
		p = p->next;
	}
	p->next = temp;
}

/*
 * Remove node from top of d_queue
 * Return type is d_queue
 */
d_queue
*remove_d_queue ( void ) {
	if ( is_d_queue_empty() )
		return NULL;
	else {
		d_queue *p = d_q;
		d_q = d_q->next;
		return p;
	}
}

/*
 * Check if d_queue is empty!
 */
int
is_d_queue_empty ( void ) {
	if ( d_q == NULL )
		return 1;
	else
		return 0;
}

/*
 * Dummy print function
 * to check if d_queue works...
 */
void
print_d_queue ( void ) {
	if ( is_d_queue_empty() )
		return;
	d_queue *p = d_q;
	while ( p != NULL ) {
		fprintf (stdout, "%d: %d\n", p->node_number, p->timestamp );
		p = p->next;
	}
}
