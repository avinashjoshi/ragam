#include "../config/commons.h"

/*
 * !!!! DO NOT EDIT BELOW THIS LINE !!!!
 */


#define REQUEST 0
#define REPLY 1

#define RETURN_FAILURE 1
#define RETURN_SUCCESS 0

#define TRUE 1
#define FALSE 0

pthread_mutex_t q_lock;
pthread_mutex_t lock;
pthread_mutex_t end_lock;

int end;

pthread_t thread_h[MAX_CLIENTS];

int node_number;
char hostname[MAX_HOST_LEN];
char filename[50];
FILE *outfile;

/* Global connection list variable */
struct node {
	int sock;
	char name[HOST_SIZE];
	int status;
} con_list [ MAX_SERVERS ];

/* Linked List for algorithm */
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

int is_connected ( char *);

void print_con_list ( void );

int all_connected ( void );

char* get_node_name_from_socket ( int );

int get_node_index ( char *);

int add_to_conlist ( char *, int );

void handle_receive ( void );

/*
 * All function def. for queue
 */
r_queue* remove_r_queue ( void );
int is_r_queue_empty ( void );
void insert_r_queue ( char * );
void print_r_queue ( void );
int get_server_id ( char * );
