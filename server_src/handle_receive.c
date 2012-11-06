/*
 * File handle_receive.c
 *
 * This handles all Receive requests
 */

#include "server_header.h"
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

/*
 * To be implemented...
 */
void
handle_receive ( void ) {
	// Check if there is data in the queue

	r_queue *q;
	int ts, node, mode;
	char *token;
	char buffer[BUFF_SIZE];
	//printf ("\n INSIDE HANDLE_RECEIVE()\n");
	while ( 1 ) {
		pthread_mutex_lock ( &q_lock );
		if ( is_r_queue_empty() == TRUE ) {
			pthread_mutex_unlock( &q_lock );
			usleep(100);
			continue;
		}
		q = remove_r_queue ();
		pthread_mutex_unlock ( &q_lock );

		fprintf ( outfile, "%s\n", q->data);

		// WRITE TO FILE

	}

	return NULL;
}
