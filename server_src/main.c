#include "server_header.h"
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>

const char *prog_name;

int
main ( int argc, char *argv[] ) {

	/* Variable Declerations */
	prog_name = get_program_name ( argv[0] );
	int port;

#ifndef SERVER_PORT
	/* Listen port not defined
	 * => Program must accept port */
	if ( argc < 2 ) {
		fprintf ( stderr, "error: %s: Port number needed to continue...\n", prog_name );
		exit ( EXIT_FAILURE );
	}
#endif

	// Convert port to integer if taken as argument
#ifndef SERVER_PORT
	port = atoi ( argv[1] );
#else
	port = SERVER_PORT;
#endif

	//parse_client_config ();

	/* 
	 * Setting defer and request 
	 * to NULL
	 */
	time_t total_start, total_stop;
	r_q = NULL;

	/*
	 * Get hostname for this node
	 * and index in con_list[]
	 */
	gethostname ( hostname, sizeof hostname);
	node_number = get_server_id ( hostname );
	if ( node_number == -1 ) {
		fprintf (stderr, "This node cannot be a server! Please check config/server.conf\n");
		exit (EXIT_FAILURE);
	}
	printf ("DATA CENTER #%d: %s\n", node_number, hostname);

	sprintf(filename, "data_centers/s_%d", node_number);
	outfile = fopen ( filename, "a" );

	if ( outfile == NULL ) {
		printf ("Please create output folder..");
		exit ( EXIT_FAILURE );
	}

	fprintf (outfile, "--- DATA CENTER #%d: %s ---\n", node_number, hostname);
	//fclose (outfile);

	/*
	 * Timestamp is the node number
	 * to eliminate same timestamp requests
	 */

	time (&total_start);

	/*
	 * Setup listen threads that will 
	 * allow other nodes talk to me
	 */
	setup_listen_thread ( port );

	/*
	 * Start handle_request thread
	pthread_t thread;
	if ( pthread_create ( &thread, NULL, handle_receive, (void *) NULL ) != 0 ) {
		fprintf ( stderr, "Oops! Could not fork handle_request() thread\n");
		exit (EXIT_FAILURE);
	}
	 */

	handle_receive();

	time (&total_stop);

	/*
	fprintf ( stdout, "Waiting for scanf...." );
	int a;
	scanf ( "%d", &a );
	*/

	/*
	while (end_compute < MAX_SERVERS - 1 )
		usleep(1000);
	printf ("\n:End = %d:\n", end_compute);
	*/

	/*
	 * Closing all sockets before quiting program
	for ( i = 0; i < MAX_SERVERS; i++ ) {
		close ( con_list[i].sock );
	}
	 */

	fclose (outfile);
	print_con_list();

	// Have to write the initial connection code here

	return EXIT_SUCCESS;
}
