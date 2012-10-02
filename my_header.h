#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Comment the line below if you
 * want to user different port
 * for different nodes
 * (Usefule while testing on local)
 */
#define LISTEN_PORT 31337

#define MAX_NODES 10

#define RETURN_FAILURE 1
#define RETURN_SUCCESS 0

#define TRUE 1
#define FALSE 0

/*
 * Comment the below line if you
 * want to disable debug mode
 */
#define _DEBUG_

#ifdef _DEBUG_
	#define DBG(x)  printf ("\n_DEBUG: "); printf x; printf("\n");
#else
	#define DBG(x)  /* Nothing here */
#endif

/* To get "program name" */
#ifndef PATH_SEPARATOR
	#define PATH_SEPARATOR '/'
#endif

/* Global connection list variable */
struct node {
	int sock;
	char name[20];
} con_list [ MAX_NODES ];

/* Function declerations */

const char* get_program_name ( char[] );

void setup_listen_thread ( int );
