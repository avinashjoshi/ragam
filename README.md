# ragam README #
================

This software implements the Ricart-Agrawala 
algorithm for distributed mutual exclusion 
with Roucairol-Carvalho optimization.

Full Details in:
1. scripts/Description_1.pdf
2. scripts/Description_22.pdf

Author: Avinash Joshi <avinash.joshi@utdallas.edu>

Folder Organization
==============================
client_src: contains source files related to the client part (Description 1)
server_src: contains source files related to the server part (Description 2)
config    : contains commons.h, client.conf & server.conf
scripts   : contains scripts that ease setup, cleanup, execution & testing of project

Before you compile / execute
==============================
1. change the value for # of clients and servers in config/commons.h
    - #define MAX_SERVERS
    - #define MAX_CLIENTS
2. you can change the port number in config/commons.h
3. make sure corresponding number of client and server entries exist
   in config/client.conf and config/server.con files respectively

Compling & Executing
============================
Method I] The easy way
----------------------
while in the main folder, execute:
  $ ./scripts/automate.sh

Method II]
----------
  $ make clobber
  $ make
Go to each node and execute the the server of client code:
server: $ ./ragam_server
client: $ ./ragam_client

Analysis / Verification
============================
Every server stores its replica in the folder ./data_center
The script -- ./script/verify.sh will check if all the copies
  are the same. Also, it checks for the the number of entries
  in each replica/data center for each node

Scripts Provided / Available
============================
There are several scripts available in the scripts folder:
- cleanup.sh  -- cleans all un-necessary files in the project
- setup.sh    -- cleanup & make
- automate.sh -- setup
                 ssh to every server & run ./ragam_server script
                 ssh to every client & run ./ragam_client script
- kill.sh     -- If your program is stuck, for any reason,
                  use this to kill all client and server script
- verify.sh   -- As described in the Analysis section

* Note: All scripts must be called from the projects home folder
          $ ./script/<script_name>

/Avinash
