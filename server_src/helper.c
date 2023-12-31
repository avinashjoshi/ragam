/*
 * File: helper.c
 *
 * Containes all helper functions
 * that may be necessary for this application
 *
 * Author: Avinash Joshi <avinash.joshi@utdallas.edu>
 */

#include "server_header.h"
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
parse_client_config ( void ) {
	struct hostent *he;
	int index = 0;
	FILE *file;
	char *c;

	file = fopen ( CLIENT_CONF, "r" );
	if ( file != NULL ) {
		char line[HOST_SIZE];
		while ( index < MAX_CLIENTS ) {
			if ( fgets ( line, sizeof line, file ) == NULL )
				break;
			c = strchr (line, '\n');
			if (c)
				*c = 0;
			/* Get host name */
			he = gethostbyname ( line );
			printf ("%d: %s %s - %ld\n", index, line, he->h_name, sizeof (he->h_name));
			con_list[index].sock = -1;
			strcpy ( con_list[index].name, he->h_name );
			con_list[index].status = FALSE;
			index++;
		}
		fclose (file);
	} else {
		perror ( CLIENT_CONF );
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
	for ( ; index < MAX_CLIENTS; index++ ) {
		if ( strcasecmp ( con_list[index].name, node ) == 0 ) {
			if ( con_list[index].status == TRUE ) {
				flag = TRUE;
				break;
			}
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
	int i = 0;
	int flag = FALSE;
	for ( ; i < MAX_CLIENTS; i++ ) {
		if ( strcasecmp ( con_list[i].name, host ) == 0 ) {
			flag = TRUE;
			break;
		}
	}
	if ( flag == FALSE ) {
		printf ("*** Something's wrong....\n");
		return -1;
	}
	con_list[i].sock = sock;
	strcpy ( con_list[i].name, host );
	con_list[i].status = TRUE;
	return i;
}

/*
 * Print connection list
 */
void
print_con_list ( void ) {
	int i;
	printf ( "\n\n CONN LIST:\n");
	for ( i = 0; i < MAX_CLIENTS; i++ ) {
		printf ( "%d : %s\n", con_list[i].sock, con_list[i].name);
	}
}

/*
 * Check if all nodes are connected
 */
int
all_connected ( void ) {
	int i = 0;
	int flag = FALSE;
	for ( ; i < MAX_CLIENTS; i++ ) {
		if ( con_list[i].status == FALSE ) {
			flag = TRUE;
			break;
		}
	}
	if ( flag )
		return FALSE;
	else
		return TRUE;
}

char *
get_node_name_from_socket ( int sock ) {
	int i = 0;
	for ( ; i < MAX_CLIENTS; i++ ) {
		if ( sock == con_list[i].sock )
			break;
	}
	return con_list[i].name;
}

int get_node_index ( char *hostname) {
	int i = 0;
	for ( ; i < MAX_CLIENTS; i++ ) {
		if ( strcasecmp ( con_list[i].name, hostname) == 0 )
			break;
	}
	return i;
}

int get_server_id ( char *hname ) {
	struct hostent *he;
	int index = 0;
	int flag = FALSE;
	int line_number = -1;
	FILE *file;
	char *c;

	file = fopen ( SERVER_CONF, "r" );
	if ( file != NULL ) {
		char line[HOST_SIZE];
		while ( index < MAX_SERVERS ) {
			if ( fgets ( line, sizeof line, file ) == NULL )
				break;
			c = strchr (line, '\n');
			if (c) {
				line_number++;
				*c = 0;
			}
			/* Get host name */
			he = gethostbyname ( line );
			if ( strcasecmp (he->h_name, hname) == 0 ) {
				flag = TRUE;
				break;
			}
			index++;
		}
		fclose (file);
	} else {
		perror ( SERVER_CONF );
	}
	if ( flag == TRUE )
		return line_number;
	else
		return -1;
}
