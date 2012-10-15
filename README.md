# ragam README #
================

This software implements the Ricart-Agrawala 
algorithm for distributed mutual exclusion 
with Roucairol-Carvalho optimization.

Full Details in Description.pdf

Author: Avinash Joshi <avinash.joshi@utdallas.edu>


Compling & Executing
============================

1. Add hostname of the nodes participating in 'config' file
2. Define the # nodes & port number in my_header.h 
3. Execute:
	$ make clobber && make
4. Create output folder
5. Log on to individual servers and run ./ragam
6. Results will be in output folder

Analysis
============================
1. The total number of messages exchanged:
	2 * ( N - 1 ) * 10 * 20 = 7200
2. Output folder
3. This is because there are like 10 nodes and in the second iteration,
	the time interval is large!!

/Avinash
