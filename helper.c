/*
 * File: helper.c
 *
 * Containes all helper functions
 * that may be necessary for this application
 *
 * Author: Avinash Joshi <avinash.joshi@utdallas.edu>
 */

#include "my_header.h"

/*
 * get_program_name takes string as input
 * and gets the file (executable) name
 * from the last '/' character
 */
const char*
get_program_name ( char argv[] ) {

	/* strrchr gives the last occurance of PATH_SEPARATOR in argv[0] */
	const char *exec_name;

	exec_name = strrchr (argv, PATH_SEPARATOR);
	if (!exec_name)
		exec_name = argv;
	else
		++exec_name;

	return exec_name;
}

/*
 * parse config file...
 */
void
parse_config ( void ) {
	int index;
	for ( index = 0; index < MAX_NODES; index++ ) {
		con_list[index].sock = 0;
		//strncpy ( node_list[index].name, "localhost", sizeof ("localhost") );
	}
	strncpy ( node_list[index].name, "net01", sizeof ("net01") );
	strncpy ( node_list[index].name, "net02", sizeof ("net02") );
}

/*
 * Checks if a node is connected
 * That is, does a socket exist to a particular node
 */
int
is_connected ( char *node) {
	int index = 0;
	int flag = FALSE;
	for ( ; index < MAX_NODES; index++ ) {
		if ( strcasecmp ( con_list[index].name, node ) == 0 ) {
			printf ("%s:", con_list[index].name);
			flag = TRUE;
			break;
		}
	}
	if ( flag == TRUE )
		return index;
	else
		return -1;
}

/*
 * Add to con_list
 */
int
add_to_conlist ( char *host, int sock ) {
	// START MUTEX
	int i = 0;
	for ( ; i < MAX_NODES; i++ ) {
	}
	// END MUTEX
	return -1;
}
