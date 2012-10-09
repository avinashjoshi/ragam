#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Comment the line below if you
 * want to user different port
 * for different nodes
 * (Usefule while testing on local)
 */
#define LISTEN_PORT 1337
#define MAX_NODES 3
#define BUFF_SIZE 1024
#define MAX_HOST_LEN 100

#define RETURN_FAILURE 1
#define RETURN_SUCCESS 0

#define TRUE 1
#define FALSE 0

#define CONFIG_FILE "config"
#define HOST_SIZE 30

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

pthread_mutex_t lock;

/* Global connection list variable */
struct node {
	int sock;
	char name[HOST_SIZE];
	int status;
} con_list [ MAX_NODES ];

/* Linked List for algorithm */
struct queue {
	char name[HOST_SIZE];
	struct queue *next;
} *root;

/* Function declerations */

const char* get_program_name ( char[] );

void setup_listen_thread ( int );

void parse_config ( void );

void setup_connect_to ( int );

int is_connected ( char *);

void print_con_list ( void );

int all_connected ( void );

char* get_node_name_from_socket ( int );

int get_node_index ( char *);

void start_compute ( void );
