/*
 * File: compute.c
 * This file has computation after forking threads
 */

#include "my_header.h"
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

void
start_compute ( void ) {
	char buffer[BUFF_SIZE];
	int timestamp = node_number;
	int i;
	sprintf ( buffer, "%d|%d|%d", REQUEST, node_number, timestamp );
	//while ( all_connected() == FALSE );
	pthread_mutex_lock ( &requesting_lock );
	is_requesting = TRUE;
	request_ts = timestamp;
	pthread_mutex_unlock ( &requesting_lock );
	for ( i = 0; i < MAX_NODES; i++ ) {
		if ( strcasecmp ( hostname, con_list[i].name ) == 0 ) {
			continue;
		}
		send ( con_list[i].sock, buffer, BUFF_SIZE, 0 );
	}
	while ( 1 ) {
		sleep (5);
		printf ("Trying to enter critical section...\n");
		pthread_mutex_lock ( &requesting_lock );
		if ( total_requests == MAX_NODES-1 ) {
			printf ("Phew... Got access\n");
			pthread_mutex_unlock ( &requesting_lock );
			break;
		}
		pthread_mutex_unlock ( &requesting_lock );
	}
	// Send all defered replies
	d_queue *ptr;
	while ( (ptr = remove_d_queue ()) != NULL ) {
		pthread_mutex_lock (&requesting_lock);
		sprintf ( buffer, "%d|%d|%d", REPLY, node_number, request_ts+5);
		printf ("SENDING %s to %s\n", buffer, get_node_name_from_socket ( con_list[ptr->node_number].sock ));
		send ( con_list[ptr->node_number].sock, buffer, BUFF_SIZE, 0);
		pthread_mutex_unlock (&requesting_lock);
	}
}
