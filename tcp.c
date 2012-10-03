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
#include <unistd.h>

#define BUFF_SIZE 1024
#define MAX_HOST_LEN 100

/*
 * Handles new socket after accept
 */
void
*handle_socket ( void *new_sock ) {
	int sock = (int) new_sock;
	fprintf ( stdout, "Yahoo! got a new connection\n" );
	close (sock);
	return NULL;
}

/*
 * Thread to block on accept and 
 * create new thread for incoming connection
 */
void
*handle_listen ( void *tport ) {

	int nodes_index = 0;
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
		printf ( "getaddrinfo()" );
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

	printf ( "socket()" );

	/* Bind to sock address */
	if ( bind ( sock, res->ai_addr, res->ai_addrlen ) == -1 ) {
		perror ( "bind()" );
		return NULL;
	}

	printf ( "BOUND....\n" );

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
			if ( pthread_create ( &thread, NULL, handle_socket, &newsock) != 0 ) {
				fprintf ( stderr, "Failed to create thread :(\n" );
				continue;
			} else {
				nodes_index ++;
			}
		}
	} while ( nodes_index < MAX_NODES );
	printf ( "Closing sockets....\n");
	close ( sock );
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

void
setup_connect_to ( int port ) {

	int index_list = 0;
	char host[MAX_HOST_LEN];
	int sock;
	struct sockaddr_in server;   /* Socket info. for server */
	struct sockaddr_in client;   /* Socket info. about us */
	int clientLen;   /* Length of client socket struct. */
	struct hostent *hp;   /* Return value from gethostbyname() */
	char buf[BUFF_SIZE];   /* Received data buffer */

	gethostname ( host, sizeof host );
	fprintf ( stdout, "Your hostname = %s\n", host );

	/* 
	 * Go through node_list and check
	 * if socket already exists for the host
	 * if not, connect & create a new thread
	 */

	for ( ; index_list < MAX_NODES; index_list++ ) {

		// Check if socket associated w/ host
		if ( (sock = is_connected ( node_list[index_list].name )) > -1 ) {
			/* Oops! looks like a socket is associated with that node */
			printf ( "Already connected");
			continue;
		}

		// Come on! You can't connect to self...
		if ( strcasecmp ( node_list[index_list].name, host ) == 0 ) {
			printf ( "Cant connect to self!" );
			continue;
		}

		// Looks like host not in con_list
		// Add to con_list
		// add_to_conlist (host, socket);
		// Create a TCP conection to the port
		/* Opening a socket */
		if ( (sock = socket ( AF_INET, SOCK_STREAM, 0)) < 0 ) {
			printf ( "Error in opening socket()\n");
		}

		/* Preparing to connect to the socket */
		bzero ( (char *) &server, sizeof server );
		server.sin_family = AF_INET;
		if ( (hp = gethostbyname(host) ) == NULL) {
			sprintf( buf, "%s: unknown host\n", host);
			printf( buf );
		}

		bcopy( hp->h_addr, &server.sin_addr, hp->h_length );
		server.sin_port = htons( (u_short) port );

		/* Try to connect */
		if ( connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
			printf ("Error: Connecting stream socket");
			continue;
		}

		/* Determine what port client's using. */
		clientLen = sizeof (client);
		if ( getsockname (sock, (struct sockaddr *) &client, (socklen_t *)&clientLen)) {
			printf ("Getting socket name");
			continue;
		}

		if ( clientLen != sizeof(client)) {
			printf ("getsockname() overwrote name structure");
			continue;
		}

		printf ("Client socket has port %hu\n", ntohs(client.sin_port));

		/* Write out message. */
		if ( send ( sock, "connect", sizeof ("connect"), 0 ) < 0 )
			printf ( "Writing on stream socket" );

		printf ("Sent...!");
	}
	// thread for handle_socket
	// to read data on that socket

	//end for
}
