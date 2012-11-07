/*
 * File: tcp.c
 *
 * This file contains all functions that help ragam
 * with creating and listening to incoming requests
 */

#include "server_header.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

/*
 * Handles new socket after accept
 */
void
*handle_socket ( void *new_sock ) {
	char buffer[BUFF_SIZE];
	int sock = (long int) new_sock;
	//fprintf ( stdout, "Entering Main Computation area.... Socket used is: %d | %s\n", sock, get_node_name_from_socket (sock) );
	while ( 1 ) {
		bzero ( buffer, BUFF_SIZE);
		recv (sock, buffer, BUFF_SIZE, 0);
		if ( strcasecmp (buffer, "" ) == 0 ) {
			continue;
		}
		fprintf (stdout, "INSERTED into Q -- %s\n", buffer);
		pthread_mutex_lock ( &q_lock );
		insert_r_queue ( buffer );
		pthread_mutex_unlock ( &q_lock );
		if ( strcasecmp (buffer, "END" ) == 0 ) {
			break;
		}
		bzero ( buffer, BUFF_SIZE);
	}
	close (sock);
	return NULL;
}

/*
 * Thread to block on accept and 
 * create new thread for incoming connection
 */
void
*handle_listen ( void *tport ) {

	pthread_mutex_lock (&lock);

	int sock;
	struct addrinfo hints, *res;
	struct sockaddr_in server_address;
	int reuseaddr = 1; // True
	int port_int = (long int) tport;
	char port_str[10];
	snprintf ( port_str, sizeof port_str, "%d", port_int );


	/* Get address info */
	memset ( &hints, 0, sizeof hints );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ( getaddrinfo ( NULL, port_str, &hints, &res ) != 0 ) {
		printf ( "getaddrinfo()" );
		pthread_mutex_unlock (&lock);
		return NULL;
	}

	/* Create a socket */
	sock = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( sock == -1 ) {
		perror ( "socket()");
		pthread_mutex_unlock (&lock);
		return NULL;
	}

	/* Enable the socket to reuse address */
	if ( setsockopt ( sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int) ) == -1 ) {
		perror ( "setsockopt()" );
		pthread_mutex_unlock (&lock);
		return NULL;
	}

	//printf ( "socket()\n" );

	/* Bind to sock address */
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (port_int);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//printf("Server-Using %s and port %d...\n", inet_ntoa(server_address.sin_addr), port_int);

	if ( bind ( sock, (struct sockaddr *) &server_address, sizeof (struct sockaddr) ) == -1 ) {
		perror ( "bind()" );
		pthread_mutex_unlock (&lock);
		return NULL;
	}

	pthread_mutex_unlock (&lock);
	//printf ( "BOUND.... " );

	/* Listen */
	if ( listen ( sock, 32 ) == -1 ) {
		perror ( "listen()" );
		return NULL;
	}

	struct hostent *he;
	struct in_addr ipv4addr;

	/* Main loop begins here */
	do {
		// accept connection here
		socklen_t size = sizeof ( struct sockaddr_in );
		struct sockaddr_in their_addr;
		//printf ("Waiting in accept()\n");
		int newsock = accept ( sock, ( struct sockaddr* ) &their_addr, &size );
		pthread_mutex_lock (&lock);
		if ( newsock == -1 ) {
			perror ( "accept()");
			pthread_mutex_unlock (&lock);
			continue;
		} else {
			DBG (( "\nReceived connection from %s on socket %d\n", inet_ntoa(their_addr.sin_addr), newsock ));
			inet_pton(AF_INET, inet_ntoa(their_addr.sin_addr), &ipv4addr);
			he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

			if ( is_connected(he->h_name) > -1 ) {
				//printf ("Server -- already connected");
				pthread_mutex_unlock (&lock);
				continue;
			}

			if ( pthread_create ( &thread_h[get_node_index(he->h_name)], NULL, handle_socket, (void *) newsock) != 0 ) {
				fprintf ( stderr, "Failed to create thread :(\n" );
				pthread_mutex_unlock (&lock);
				continue;
			} else {
				printf ("ADDING %s\n", he->h_name);
				add_to_conlist (he->h_name, newsock);
				pthread_mutex_unlock (&lock);
			}
		}
		pthread_mutex_lock (&lock);
		if ( all_connected() ) {
			pthread_mutex_unlock (&lock);
			break;
		}
		pthread_mutex_unlock (&lock);
	} while ( 1 );
	//printf ( "Closing sockets....\n");
	//close ( sock );
	pthread_exit ( NULL );
}

void
setup_listen_thread ( int port ) {
	int err;
	DBG (("Port before = %d", port));
	pthread_t tcp_pid;
	// void *status; // For pthread_join

	DBG (("creating thread..."));

	err = pthread_create ( &tcp_pid, NULL, handle_listen, (void *) port );

	if ( err != 0 )
		fprintf ( stderr, "Unable to create thread :(\n");
	else
		fprintf ( stderr, "Thread created....!\n");

	//pthread_join ( tcp_pid, &status );

	DBG (("created thread"));
	return;
}
