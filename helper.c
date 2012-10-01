/*
 * File: helper.c
 *
 * Containes all helper functions
 * that may be necessary for this application
 *
 * Author: Avinash Joshi <avinash.joshi@utdallas.edu>
 */

#include "my_header.h"

const char *com_list[] = {"help", "makemaster", "connect", "NULL"};

/*
 * is_command takes string as argument and checks
 * if that is present in com_list[] array
 * 
 * Return 0 if not present else returns 1
 */
int
is_command ( char *com ) {
	int i = 0;
	while (strcmp ("NULL", com_list[i]) != 0) {
		if (strcmp (com, com_list[i]) == 0)
			return 1;
		i++;
	}
	return 0;
}

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
