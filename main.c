#include "my_header.h"

const char *prog_name;

void
print_help ( void ) {
	fprintf ( stdout, "%s: takes 1 argument - makemaster, connect\n", prog_name );
	return;
}

int
main ( int argc, char *argv[] ) {

	/* Variable Declerations */
	prog_name = get_program_name ( argv[0] );

	/* Program must have atleast one argument */
	if ( argc < 2 ) {
		fprintf ( stderr, "error: %s: must have atleast one argument\n", prog_name );
		print_help ();
		exit ( EXIT_FAILURE );
	}

	/* Is the second argument a valid command? */
	if ( !is_command ( argv[1] )) {
		fprintf ( stderr, "error: %s: %s is an invalid command\n", prog_name, argv[1] );
		exit ( EXIT_FAILURE );
	}

	/* Creating a master? */
	if ( strcmp ( argv[1], "makemaster" ) == 0 ) {
#ifndef LISTEN_PORT
		if ( argc != 3 ) {
			fprintf ( stderr, "error: %s: makemaster command needs port_number\n", prog_name );
			exit ( EXIT_FAILURE );
		}
#endif
		// Do Make server
	}

	/* Connecting to a master? */
	if ( strcmp ( argv[1], "connect" ) == 0 ) {
		if ( argc < 3 ) {
#ifndef LISTEN_PORT
			fprintf ( stderr, "error: %s: connect command needs \"IP port_number\"\n", prog_name );
#else
			fprintf ( stderr, "error: %s: connect command needs IP\n", prog_name );
#endif
			exit ( EXIT_FAILURE );
		}

#ifndef LISTEN_PORT
		if ( argc != 4 ) {
			fprintf ( stderr, "error: %s: connect command needs \"IP port_number\"\n", prog_name );
			exit ( EXIT_FAILURE );
		}
#endif
		// Connect to master
	}

	return EXIT_SUCCESS;
}
