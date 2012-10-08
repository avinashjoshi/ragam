/*
 * File: helper.c
 *
 * Containes all helper functions
 * that may be necessary for this application
 *
 * Author: Avinash Joshi <avinash.joshi@utdallas.edu>
 */

#include "my_header.h"
#include <netdb.h>
#include <netinet/in.h>

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
 * Returns the IP address of host
void
get_host_ip ( char *hostname ) {
	he = gethostbyname (hostname);
	return; 
}
 */

/*
 * parse config file...
 */
void
parse_config ( void ) {
	struct hostent *he;
	int index = 0;
	for ( index = 0; index < MAX_NODES; index++ ) {
		con_list[index].sock = -1;
		strncpy ( node_list[index].name, "NULL", sizeof ("NULL") );
	}
	FILE *file;
	file = fopen ( CONFIG_FILE, "r" );
	index = 0;
	char *c;
	if ( file != NULL ) {
		char line[HOST_SIZE];
		while ( index < MAX_NODES ) {
			if ( fgets ( line, sizeof line, file ) == NULL )
				break;
			c = strchr (line, '\n');
			if (c)
				*c = 0;
			/* Get host name */
			he = gethostbyname ( line );
			printf ("%d: %s %s - %d\n", index, line, he->h_name, sizeof (he->h_name));
			strcpy ( node_list[index].name, he->h_name );
			index++;
		}
		fclose (file);
	} else {
		perror ( CONFIG_FILE );
	}
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
		if ( con_list[i].sock == -1 )
			break;
	}
	con_list[i].sock = sock;
	strncpy ( con_list[i].name, host, sizeof (host) );
	// END MUTEX
	return i;
}
