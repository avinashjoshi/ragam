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
	//printf ("\n INSIDE HANDLE_RECEIVE()\n");
	while ( 1 ) {
		// Check if all clients sent END msg
		pthread_mutex_lock ( &end_lock );
		if ( end == MAX_CLIENTS ) {
			pthread_mutex_unlock ( &end_lock );
			break;
		}
		pthread_mutex_unlock ( &end_lock );
		// Check if queue is empty
		pthread_mutex_lock ( &q_lock );
		if ( is_r_queue_empty() == TRUE ) {
			pthread_mutex_unlock( &q_lock );
			usleep(100);
			continue;
		}
		//if not, remove data from queue
		q = remove_r_queue ();
		pthread_mutex_unlock ( &q_lock );

		if ( strcasecmp (q->data, "END") == 0 ) {
			pthread_mutex_lock ( &end_lock );
			end++;
			pthread_mutex_unlock ( &end_lock );
			continue;
		}

		outfile = fopen ( filename, "a" );
		fprintf ( outfile, "%s\n", q->data);
		fclose (outfile);

		// WRITE TO FILE

	}

	return;
}
