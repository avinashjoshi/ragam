/*
 * File: compute.c
 * This file has computation after forking threads
 */

#include "my_header.h"
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

void
send_deferred_replies ( void ) {
	// Send all defered replies
	char buffer[BUFF_SIZE];
	d_queue *ptr;
	pthread_mutex_lock (&dq_lock);
	while ( (ptr = remove_d_queue ()) != NULL ) {
		sprintf ( buffer, "%d|%d|%d", REPLY, node_number, global_ts);
		printf ("SENDING DEFERRED REPLY %s to %s\n", buffer, get_node_name_from_socket ( con_list[ptr->node_number].sock ));
		send ( con_list[ptr->node_number].sock, buffer, BUFF_SIZE, 0);
	}
	pthread_mutex_unlock (&dq_lock);
}

void
start_compute ( void ) {
	char buffer[BUFF_SIZE];
	int i, attempt;
	for ( attempt = 1 ; attempt <= 10; attempt++ ) {
		printf ("\n===== ATTEMPT %d =====\n", attempt );
		//while ( all_connected() == FALSE );
		pthread_mutex_lock ( &requesting_lock );
		is_requesting = TRUE;
		pthread_mutex_unlock ( &requesting_lock );
		pthread_mutex_lock (&ts_lock);
		request_ts = global_ts;
		sprintf ( buffer, "%d|%d|%d", REQUEST, node_number, global_ts );
		pthread_mutex_unlock (&ts_lock);
		for ( i = 0; i < MAX_NODES; i++ ) {
			if ( strcasecmp ( hostname, con_list[i].name ) == 0 ) {
				continue;
			}
			send ( con_list[i].sock, buffer, BUFF_SIZE, 0 );
			printf ("SENDING REQUEST TO %s: %s\n", con_list[i].name, buffer);
			pthread_mutex_lock (&ts_lock);
			global_ts++;
			pthread_mutex_unlock (&ts_lock);
		}
		printf ("\nTrying to enter critical section...\n");
		while ( 1 ) {
			sleep (10);
			printf ("+");
			pthread_mutex_lock ( &requesting_lock );
			if ( total_requests == MAX_NODES-1 ) {
				pthread_mutex_unlock ( &requesting_lock );
				pthread_mutex_lock ( &critical_lock );
				is_in_critical = TRUE;
				pthread_mutex_unlock ( &critical_lock);
				printf ("Phew... Got acces, sleeping for 3 seconds\n");
				sleep (3);
				pthread_mutex_lock ( &critical_lock );
				is_in_critical = FALSE;
				pthread_mutex_unlock ( &critical_lock );
				break;
			}
			pthread_mutex_unlock ( &requesting_lock );
		} // End While
		send_deferred_replies ();
		pthread_mutex_lock ( &requesting_lock );
		is_requesting = FALSE;
		total_requests = 0;
		pthread_mutex_unlock ( &requesting_lock );
	}
	// For odd and even nodes
}
