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

#define REQUEST 0
#define REPLY 1

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
pthread_mutex_t r_lock;
pthread_mutex_t q_lock;

pthread_mutex_t requesting_lock;
pthread_mutex_t critical_lock;

int is_in_critical, is_requesting;

/* Global connection list variable */
struct node {
	int sock;
	char name[HOST_SIZE];
	int status;
} con_list [ MAX_NODES ];

/* Linked List for algorithm */
/*
 * Deferred queue list
 */
typedef struct d_linked_list d_queue;
struct d_linked_list {
	int node_number;
	int timestamp;
	d_queue *next;
} *d_q;

/*
 * This holds all requests 
 * coming into socket
 */
typedef struct r_linked_list r_queue;
struct r_linked_list {
	char data[BUFF_SIZE];
	r_queue *next;
} *r_q;

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

int add_to_conlist ( char *, int );

void *handle_receive ( void * );

/*
 * All function def. for queue
 */
r_queue* remove_r_queue ( void );
int is_r_queue_empty ( void );
void insert_r_queue ( char * );
void print_r_queue ( void );

d_queue* remove_d_queue ( void );
int is_d_queue_empty ( void );
void insert_d_queue ( int, int );
void print_d_queue ( void );
