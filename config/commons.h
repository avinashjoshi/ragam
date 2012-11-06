#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Comment the line below if you
 * want to user different port
 * for different nodes
 * (Usefule while testing on local)
 */

#define CLIENT_PORT 56744
#define SERVER_PORT 56744

#define SERVER_CONF "config/server.conf"
#define CLIENT_CONF "config/client.conf"
#define MAX_SERVERS 3
#define MAX_CLIENTS 6

/*
 * !!!! DO NOT EDIT BELOW THIS LINE !!!!
 */

#define BUFF_SIZE 1024
#define MAX_HOST_LEN 100
#define HOST_SIZE 30

/*
 * Comment the below line if you
 * want to disable debug mode
 */
//#define _DEBUG_

#ifdef _DEBUG_
	#define DBG(x)  printf ("\n_DEBUG: "); printf x; printf("\n");
#else
	#define DBG(x)  /* Nothing here */
#endif

/* To get "program name" */
#ifndef PATH_SEPARATOR
	#define PATH_SEPARATOR '/'
#endif
