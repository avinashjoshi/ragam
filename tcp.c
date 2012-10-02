/*
 * File: tcp.c
 *
 * This file contains all functions that help ragam
 * with creating and listening to incoming requests
 */

#include "my_header.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
 * Handles new socket after accept
 */
void
*handle_accept ( void *new_sock ) {
	fprintf ( stdout, "Yahoo! got a new connection\n" );
	return NULL;
}

/*
 * Thread to block on accept and 
 * create new thread for incoming connection
 */
void
*handle_listen ( void *tport ) {

	int sock;
	pthread_t thread;
	struct addrinfo hints, *res;
	int reuseaddr = 1; // True
	int port_int = (int) tport;
	char port_str[10];
	snprintf ( port_str, sizeof port_str, "%d", port_int );

	/* Get address info */
	memset ( &hints, 0, sizeof hints );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ( getaddrinfo ( NULL, port_str, &hints, &res ) != 0 ) {
		perror ( "getaddrinfo()" );
		return NULL;
	}

	/* Create a socket */
	sock = socket ( res->ai_family, res->ai_socktype, res->ai_protocol );
	if ( sock == -1 ) {
		perror ( "socket()");
		return NULL;
	}

	/* Enable the socket to reuse address */
	if ( setsockopt ( sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int) ) == -1 ) {
		perror ( "setsockopt()" );
		return NULL;
	}

	/* Bind to sock address */
	if ( bind ( sock, res->ai_addr, res->ai_addrlen ) == -1 ) {
		perror ( "bind()" );
		return NULL;
	}

	freeaddrinfo ( res );

	/* Listen */
	if ( listen ( sock, MAX_NODES ) == -1 ) {
		perror ( "listen()" );
		return NULL;
	}

	/* Main loop begins here */
	do {
		// accept connection here
		size_t size = sizeof ( struct sockaddr_in );
		struct sockaddr_in their_addr;
		int newsock = accept ( sock, ( struct sockaddr* ) &their_addr, &size );
		if ( newsock == -1 ) {
			perror ( "accept()");
		}
		else {
			DBG (( "Received connection from %s on port %d", inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port)));
			if ( pthread_create ( &thread, NULL, handle_accept, &newsock) != 0 ) {
				fprintf ( stderr, "Failed to create thread :(\n" );
			}
		}
	} while ( 1 );
	pthread_exit ( NULL );
}

void
setup_listen_thread ( int port ) {
	int err;
	DBG (("Port before = %d", port));
	pthread_t tcp_pid;
	// void *status; // For pthread_join

	DBG (("creating thread..."));

	err = pthread_create ( &tcp_pid, NULL, &handle_listen, (void *) port );

	if ( err != 0 )
		fprintf ( stderr, "Unable to create thread :(\n");
	else
		fprintf ( stderr, "Thread created....!\n");

	//pthread_join ( tcp_pid, &status );

	DBG (("created thread"));
	return;
}
