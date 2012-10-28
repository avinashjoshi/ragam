/*
 * File: compute.c
 * This file has computation after forking threads
 */

#include "my_header.h"
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/time.h>

void
send_deferred_replies ( void ) {
	// Send all defered replies
	char buffer[BUFF_SIZE];
	d_queue *ptr;
	pthread_mutex_lock (&dq_lock);
	while ( (ptr = remove_d_queue ()) != NULL ) {
		sprintf ( buffer, "%d|%d|%d", REPLY, node_number, global_ts);
		printf ("SENDING DEFERRED REPLY %s to %s\n", buffer, get_node_name_from_socket ( con_list[ptr->node_number].sock ));
		send ( con_list[ptr->node_number].sock, buffer, BUFF_SIZE, 0);
	}
	pthread_mutex_unlock (&dq_lock);
}

long long
timeval_diff(struct timeval *difference,
		struct timeval *end_time,
		struct timeval *start_time
		)
{
	struct timeval temp_diff;

	if(difference==NULL)
	{
		difference=&temp_diff;
	}

	difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
	difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

	/* Using while instead of if below makes the code slightly more robust. */

	while(difference->tv_usec<0)
	{
		difference->tv_usec+=1000000;
		difference->tv_sec -=1;
	}

	return 1000000LL*difference->tv_sec+
		difference->tv_usec;

} /* timeval_diff() */

void
start_compute ( void ) {
	char buffer[BUFF_SIZE];
	int i, attempt;
	//time_t start, stop;

	struct timeval earlier;
	struct timeval later;
	struct timeval interval;

	int unit = 100;
	int sleep_time;

	/* First Phase... */
	fprintf ( fp, "Time between request and entering CS:\n\n");
	fprintf ( fp, "======= ALL NODES ======\n");
	fprintf ( fp, "ATTEMPT\t\tTIME(u~sec)\n");
	for ( attempt = 1 ; attempt <= 20; attempt++ ) {
		printf ("\n===== ATTEMPT %d =====\n", attempt );
		//while ( all_connected() == FALSE );
		pthread_mutex_lock ( &requesting_lock );
		is_requesting = TRUE;
		pthread_mutex_unlock ( &requesting_lock );
		pthread_mutex_lock (&ts_lock);
		request_ts = global_ts;
		sprintf ( buffer, "%d|%d|%d", REQUEST, node_number, global_ts );
		pthread_mutex_unlock (&ts_lock);
		/*
		 * Requesting to access critical section
		 */
		//time(&start);
		gettimeofday(&earlier,NULL);
		for ( i = 0; i < MAX_NODES; i++ ) {
			/*
			 * Dont send to self!!!!
			 */
			if ( strcasecmp ( hostname, con_list[i].name ) == 0 ) {
				continue;
			}
			send ( con_list[i].sock, buffer, BUFF_SIZE, 0 );
			printf ("SENDING REQUEST TO %s: %s\n", con_list[i].name, buffer);
			pthread_mutex_lock (&ts_lock);
			global_ts++;
			pthread_mutex_unlock (&ts_lock);
			pthread_mutex_lock ( &analysis_lock );
			total_messages++;
			pthread_mutex_unlock ( &analysis_lock );
		}
		printf ("\nTrying to enter critical section...\n");
		while ( 1 ) {
			sleep_time = ((rand() % 10 ) + 10 ) * unit;
			usleep (sleep_time);
			pthread_mutex_lock ( &requesting_lock );
			if ( total_requests == MAX_NODES-1 ) {
				pthread_mutex_unlock ( &requesting_lock );
				pthread_mutex_lock ( &critical_lock );
				is_in_critical = TRUE;
				pthread_mutex_unlock ( &critical_lock);
				//time(&stop);
				gettimeofday(&later,NULL);
				printf ("Phew... Got access in %lld u~seconds.\n sleeping for 300 useconds\n", timeval_diff(NULL,&later,&earlier));
				fprintf ( fp, "%d\t\t%lld", attempt, timeval_diff(NULL,&later,&earlier));
				usleep (300);
				pthread_mutex_lock ( &critical_lock );
				is_in_critical = FALSE;
				pthread_mutex_unlock ( &critical_lock );
				break;
			}
			pthread_mutex_unlock ( &requesting_lock );
		} // End While
		send_deferred_replies ();
		pthread_mutex_lock ( &requesting_lock );
		is_requesting = FALSE;
		total_requests = 0;
		pthread_mutex_unlock ( &requesting_lock );
	}
	// For odd and even nodes
	fprintf ( fp, "\n======= EVEN/ODD NODES ======\n");
	fprintf ( fp, "ATTEMPT\t\tTIME(u~sec)\n");
	for ( attempt = 1 ; attempt <= 20; attempt++ ) {
		printf ("\n===== ATTEMPT %d =====\n", attempt );
		//while ( all_connected() == FALSE );
		pthread_mutex_lock ( &requesting_lock );
		is_requesting = TRUE;
		pthread_mutex_unlock ( &requesting_lock );
		pthread_mutex_lock (&ts_lock);
		request_ts = global_ts;
		sprintf ( buffer, "%d|%d|%d", REQUEST, node_number, global_ts );
		pthread_mutex_unlock (&ts_lock);
		/*
		 * Requesting to access critical section
		 */
		//time(&start);
		gettimeofday(&earlier,NULL);
		for ( i = 0; i < MAX_NODES; i++ ) {
			/*
			 * Dont send to self!!!!
			 */
			if ( strcasecmp ( hostname, con_list[i].name ) == 0 ) {
				continue;
			}
			send ( con_list[i].sock, buffer, BUFF_SIZE, 0 );
			printf ("SENDING REQUEST TO %s: %s\n", con_list[i].name, buffer);
			pthread_mutex_lock (&ts_lock);
			global_ts++;
			pthread_mutex_unlock (&ts_lock);
			pthread_mutex_lock ( &analysis_lock );
			total_messages++;
			pthread_mutex_unlock ( &analysis_lock );
		}
		printf ("\nTrying to enter critical section...\n");
		while ( 1 ) {
			if ( node_number % 2 == 0 ) {
				sleep_time = ((rand() % 10 ) + 40 ) * unit;
				usleep (sleep_time);
			} else {
				sleep_time = ((rand() % 10 ) + 10 ) * unit;
				usleep (sleep_time);
			}
			pthread_mutex_lock ( &requesting_lock );
			if ( total_requests == MAX_NODES-1 ) {
				pthread_mutex_unlock ( &requesting_lock );
				pthread_mutex_lock ( &critical_lock );
				is_in_critical = TRUE;
				pthread_mutex_unlock ( &critical_lock);
				//time(&stop);
				gettimeofday(&later,NULL);
				printf ("Phew... Got access in %lld seconds.\n sleeping for 3 useconds\n", timeval_diff(NULL,&later,&earlier));
				fprintf ( fp, "%d\t\t%lld", attempt, timeval_diff(NULL,&later,&earlier));
				usleep (300);
				pthread_mutex_lock ( &critical_lock );
				is_in_critical = FALSE;
				pthread_mutex_unlock ( &critical_lock );
				break;
			}
			pthread_mutex_unlock ( &requesting_lock );
		} // End While
		send_deferred_replies ();
		pthread_mutex_lock ( &requesting_lock );
		is_requesting = FALSE;
		total_requests = 0;
		pthread_mutex_unlock ( &requesting_lock );
	}
}
