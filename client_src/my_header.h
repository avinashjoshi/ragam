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

int end_compute;
pthread_t thread_h[MAX_CLIENTS];

int seq_number;

pthread_mutex_t end_lock;

pthread_mutex_t lock;
pthread_mutex_t dq_lock;
pthread_mutex_t q_lock;
pthread_mutex_t ts_lock;

pthread_mutex_t requesting_lock;
pthread_mutex_t critical_lock;

pthread_mutex_t analysis_lock;

int is_in_critical, is_requesting;
int request_ts, total_requests;
int global_ts;

int total_messages;

int node_number;
char hostname[MAX_HOST_LEN];
char filename[50];
FILE *fp;

/* Global connection list variable */
struct node {
	int sock;
	char name[HOST_SIZE];
	int status;
} con_list [ MAX_CLIENTS ], serv_list[MAX_SERVERS];

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

void parse_server_config ( void );
void connect_to_servers ( int );
int add_to_servlist ( char *, int );
int all_serv_connected ( void );
