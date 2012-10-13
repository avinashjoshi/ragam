/*
 * File handle_receive.c
 *
 * This handles all Receive requests
 */

#include "my_header.h"
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

/*
 * To be implemented...
 */
void
*handle_receive ( void *arg ) {
	// Check if there is data in the queue

	r_queue *q;
	int ts, node, mode;
	char *token;
	char buffer[BUFF_SIZE];
	printf ("\n INSIDE HANDLE_RECEIVE()\n");
	while ( 1 ) {
		while ( is_r_queue_empty() == TRUE );
		pthread_mutex_lock ( &q_lock );
		q = remove_r_queue ();
		pthread_mutex_unlock ( &q_lock );
		printf ( "===> Pop() - %s\n", q->data );
		token = strtok ( q->data, "|");
		mode = atoi(token);
		token = strtok (NULL, "|");
		node = atoi(token);
		token = strtok (NULL, "|");
		ts = atoi(token);
		if ( mode == REPLY ) {
			pthread_mutex_lock ( &requesting_lock );
			total_requests++;
			pthread_mutex_unlock ( &requesting_lock );
			continue;
		}
		pthread_mutex_lock ( &requesting_lock );
		pthread_mutex_lock ( &critical_lock );
		if ( ( is_in_critical == FALSE && 
			 is_requesting == FALSE ) ||
			 request_ts > ts
		   ) {
			sprintf ( buffer, "%d|%d|%d", REPLY, node_number, request_ts+5);
			printf ("\nSENDING %s to %s\n", buffer, get_node_name_from_socket ( con_list[node].sock ));
			send ( con_list[node].sock, buffer, BUFF_SIZE, 0);
		} else {
			insert_d_queue ( node, ts );
		}
		pthread_mutex_unlock ( &requesting_lock );
		pthread_mutex_unlock ( &critical_lock );
	}

	return NULL;
}
