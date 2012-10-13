/*
 * File: r_queue.c
 *
 * This file contains all functions to manupalate r_queue
 */

#include "my_header.h"

/*
 * Insert element (node) to
 * top of r_queue
 */
void
insert_r_queue ( char *text ) {
	r_queue *p = r_q;
	r_queue *temp = malloc ( sizeof(r_queue) );
	// Creating the temp r_queue
	strncpy ( temp->data, text, BUFF_SIZE );
	temp->next = NULL;
	if ( p == NULL ) {
		r_q = temp;
		return;
	}
	while ( p->next != NULL ) {
		p = p->next;
	}
	p->next = temp;
}

/*
 * Remove node from top of r_queue
 * Return type is r_queue
 */
r_queue
*remove_r_queue ( void ) {
	if ( is_r_queue_empty() )
		return NULL;
	else {
		r_queue *p = r_q;
		r_q = r_q->next;
		return p;
	}
}

/*
 * Check if r_queue is empty!
 */
int
is_r_queue_empty ( void ) {
	if ( r_q == NULL )
		return 1;
	else
		return 0;
}

/*
 * Dummy print function
 * to check if r_queue works...
 */
void
print_r_queue ( void ) {
	if ( is_r_queue_empty() )
		return;
	r_queue *p = r_q;
	while ( p != NULL ) {
		fprintf (stdout, "%s\n", p->data );
		p = p->next;
	}
}
