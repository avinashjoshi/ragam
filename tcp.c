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
	fprintf ( stdout, "Entering Main Computation area.... Socket used is: %d | %s\n", sock, get_node_name_from_socket (sock) );
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
	char hostname[MAX_HOST_LEN];
	pthread_t thread;
	struct addrinfo hints, *res;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int reuseaddr = 1; // True
	int port_int = (int) tport;
	char port_str[10];
	snprintf ( port_str, sizeof port_str, "%d", port_int );

	gethostname ( hostname, sizeof hostname );

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

	printf ( "socket()\n" );

	/* Bind to sock address */
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (port_int);
	server_address.sin_addr.s_addr = INADDR_ANY;

	printf("Server-Using %s and port %d...\n", inet_ntoa(server_address.sin_addr), port_int);

	if ( bind ( sock, (struct sockaddr *) &server_address, sizeof (struct sockaddr) ) == -1 ) {
		perror ( "bind()" );
		pthread_mutex_unlock (&lock);
		return NULL;
	}

	add_to_conlist (hostname, sock);
	pthread_mutex_unlock (&lock);
	printf ( "BOUND.... " );

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
		int size = sizeof ( struct sockaddr_in );
		struct sockaddr_in their_addr;
		printf ("Waiting in accept()\n");
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
			printf ("\nADDING %s TO CON_LIST\n", he->h_name);

			if ( is_connected(he->h_name) > -1 ) {
				printf ("Server -- already connected");
				pthread_mutex_unlock (&lock);
				continue;
			}

			if ( pthread_create ( &thread, NULL, handle_socket, newsock) != 0 ) {
				fprintf ( stderr, "Failed to create thread :(\n" );
				pthread_mutex_unlock (&lock);
				continue;
			} else {
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
	int conn_count = 0;

	gethostname ( host, sizeof host );
	fprintf ( stdout, "Your hostname = %s\n", host );

	/* 
	 * Go through con_list and check
	 * if socket already exists for the host
	 * if not, connect & create a new thread
	 */

	do {
		sleep(5);
		for ( index_list = 0; index_list < MAX_NODES; index_list++ ) {

			printf ( "\nTrying %s: ", con_list[index_list].name );

			// Come on! You can't connect to self...
			if ( strcasecmp ( con_list[index_list].name, host ) == 0 ) {
				printf ( "Cant connect to self!" );
				continue;
			}

			// Check if socket associated w/ host
			// MUTEX
			pthread_mutex_lock (&lock);
			if ( is_connected ( con_list[index_list].name ) > -1 ) {
				/* Oops! looks like a socket is associated with that node */
				printf ( "Already connected");
				conn_count++;
				pthread_mutex_unlock(&lock);
				continue;
			}
			// MUTEX

			/* Preparing to connect to the socket */
			if ( (hp = gethostbyname(con_list[index_list].name) ) == NULL) {
				sprintf( buf, "%s: unknown host\n", host);
				printf( buf );
				pthread_mutex_unlock(&lock);
				continue;
			}

			bzero ( (char *) &server, sizeof server );
			server.sin_family = AF_INET;
			server.sin_port = htons( port );
			server.sin_addr = *((struct in_addr *) hp->h_addr);

			//zero the rest of the struct
			memset(&(server.sin_zero), '\0', 8);

			// Looks like host not in con_list
			// Create a TCP conection to the port
			/* Opening a socket */
			if ( (sock = socket ( AF_INET, SOCK_STREAM, 0)) < 0 ) {
				perror ( "socket()");
				pthread_mutex_unlock(&lock);
				continue;
			}

			/* Try to connect */
			if ( connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr)) < 0 ) {
				perror ("connect()");
				close(sock);
				pthread_mutex_unlock(&lock);
				continue;
			} else {
				printf ("Connected!");
				// Add to con_list
				// MUTEX
				add_to_conlist (con_list[index_list].name, sock);
				// MUTEX
			}
			pthread_mutex_unlock(&lock);

			/* Determine what port client's using. */
			/*
			   clientLen = sizeof (client);
			   if ( getsockname (sock, (struct sockaddr *) &client, (socklen_t *)&clientLen)) {
			   printf ("Getting socket name");
			   close(sock);
			   continue;
			   }

			   if ( clientLen != sizeof(client)) {
			   printf ("getsockname() overwrote name structure");
			   close(sock);
			   continue;
			   }

			   printf ("Client socket has port %hu\n", ntohs(client.sin_port));
			   */

			/* Write out message. 
			   if ( send ( sock, "connect", sizeof ("connect"), 0 ) < 0 )
			   printf ( "Writing on stream socket" );
			   */

			conn_count++;
		} //end for()
		pthread_mutex_lock (&lock);
		if ( all_connected() ) {
			pthread_mutex_unlock (&lock);
			break;
		}
		pthread_mutex_unlock (&lock);
	} while ( 1 ); // end while()
	// thread for handle_socket
	// to read data on that socket
}
