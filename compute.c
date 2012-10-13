/*
 * File: compute.c
 * This file has computation after forking threads
 */

#include "my_header.h"
#include <unistd.h>
#include <netdb.h>

void
start_compute ( void ) {
	char hostname[MAX_HOST_LEN];
	char buffer[BUFF_SIZE];
	gethostname(hostname, sizeof hostname );;
	int node_number = get_node_index (hostname );
	int timestamp = node_number;
	int i;
	sprintf ( buffer, "%d|%d|%d", REQUEST, timestamp, node_number );
	while ( all_connected() == FALSE );
	for ( i = 0; i < MAX_NODES; i++ ) {
		if ( strcasecmp ( hostname, con_list[i].name ) == 0 ) {
			continue;
		}
		send ( con_list[i].sock, buffer, BUFF_SIZE, 0 );
	}
}
