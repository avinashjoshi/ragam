#include "my_header.h"
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>

const char *prog_name;

int
main ( int argc, char *argv[] ) {

	/* Variable Declerations */
	prog_name = get_program_name ( argv[0] );
	int port;
	int server_port;

#ifndef CLIENT_PORT
	/* Listen port not defined
	 * => Program must accept port */
	if ( argc < 2 ) {
		fprintf ( stderr, "error: %s: Port number needed to continue...\n", prog_name );
		exit ( EXIT_FAILURE );
	}
#endif

	// Convert port to integer if taken as argument
#ifndef CLIENT_PORT
	port = atoi ( argv[1] );
#else
	port = CLIENT_PORT;
#endif

	server_port = SERVER_PORT;

	int i;
	seq_number = 0;
	end_compute = 0;

	parse_config ();
	parse_server_config ();

	/* 
	 * Setting defer and request 
	 * to NULL
	 */
	time_t total_start, total_stop;
	d_q = r_q = NULL;
	is_in_critical = FALSE;
	is_requesting = FALSE;
	total_requests = 0;
	total_messages = 0;

	/*
	 * Get hostname for this node
	 * and index in con_list[]
	 */
	gethostname ( hostname, sizeof hostname);
	node_number = get_node_index ( hostname );

	sprintf(filename, "result/%d", node_number);
	fp = fopen ( filename, "a" );

	if ( fp == NULL ) {
		printf ("Please create output folder..");
		exit ( EXIT_FAILURE );
	}

	/*
	 * Timestamp is the node number
	 * to eliminate same timestamp requests
	 */
	int timestamp = node_number;
	global_ts = timestamp;

	time (&total_start);

	connect_to_servers (server_port);

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
	 * Start handle_request thread
	 */
	pthread_t thread;
	if ( pthread_create ( &thread, NULL, handle_receive, (void *) NULL ) != 0 ) {
		fprintf ( stderr, "Oops! Could not fork handle_request() thread\n");
		exit (EXIT_FAILURE);
	}

	/*
	 * start_compute() starts the main computation
	 * like sending the first REQUEST
	 */
	start_compute();

	time (&total_stop);

	/*
	fprintf ( stdout, "Waiting for scanf...." );
	int a;
	scanf ( "%d", &a );
	*/

	/*
	for ( i = 0; i < MAX_CLIENTS; i++ )
		send ( con_list[i].sock, "END", BUFF_SIZE, 0 );
	*/

	while (end_compute < MAX_CLIENTS - 1 )
		usleep(1000);
	printf ("\n:End = %d:\n", end_compute);

	/*
	int i;
	(void) pthread_join (thread, NULL);
	for ( i = 0; i < MAX_CLIENTS; i++ )
		(void) pthread_join (thread_h[i], NULL);
	while (end_compute < MAX_CLIENTS - 1 )
		usleep(1000);

		*/
	/*
	 * Closing all sockets before quiting program
	for ( i = 0; i < MAX_CLIENTS; i++ ) {
		close ( con_list[i].sock );
	}
	 */

	print_con_list();
	printf ("\nREQUEST QUEUE\n");
	print_r_queue ();
	printf ("\nDEFERRED QUEUE\n");
	print_d_queue ();
	printf ("\nAnalysis\n====================================\n");
	printf ("Total # messages exchanged: %d\n", total_messages);
	printf ("Total Execution time: %.0f\n", difftime(total_stop, total_start));
	fprintf ( fp, "\n\nAnalysis\n====================================\n");
	fprintf ( fp, "Total # messages exchanged: %d\n", total_messages);
	fprintf ( fp, "Total Execution time: %.0f\n", difftime(total_stop, total_start));
	fclose ( fp );

	// Have to write the initial connection code here

	return EXIT_SUCCESS;
}
