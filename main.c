#include "my_header.h"
#include <pthread.h>

const char *prog_name;

int
main ( int argc, char *argv[] ) {

	/* Variable Declerations */
	prog_name = get_program_name ( argv[0] );
	int port;

#ifndef LISTEN_PORT
	/* Listen port not defined
	 * => Program must accept port */
	if ( argc < 2 ) {
		fprintf ( stderr, "error: %s: Port number needed to continue...\n", prog_name );
		exit ( EXIT_FAILURE );
	}
#endif

	// Convert port to integer if taken as argument
#ifndef LISTEN_PORT
	port = atoi ( argv[1] );
#else
	port = LISTEN_PORT;
#endif

	parse_config ();

	/*
	 * Setup listen threads that will 
	 * allow other nodes talk to me
	 */
	setup_listen_thread ( port );

	/*
	 * Now that a thread is ready to listen,
	 * we will connect to all other clints...
	 */
	setup_connect_to ( port );

	/*
	 * start_compute() starts the main computation
	 * like sending the first REQUEST
	 */
	start_compute();

	/*
	 * Closing all sockets before quiting program
	int i;
	for ( i = 0; i < MAX_NODES; i++ ) {
		close ( con_list[i].sock );
	}
	 */

	fprintf ( stdout, "Waiting for scanf...." );
	int a;
	scanf ( "%d", &a );
	print_con_list();

	// Have to write the initial connection code here

	return EXIT_SUCCESS;
}
