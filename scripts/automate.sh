#!/bin/sh
ROOT_DIR="/home/004/a/ax/axj107420/aos/ragam"
clients=`cat $ROOT_DIR/config/commons.h | grep MAX_CLIENTS | cut -f3 -d" "`
servers=`cat $ROOT_DIR/config/commons.h | grep MAX_SERVERS | cut -f3 -d" "`
total=`expr $clients + $servers`
#echo "-------  :file:  ---- :stdout:"
echo "Cleaning up..."
#cd $ROOT_DIR && ./scripts/kill.sh
cd $ROOT_DIR && ./scripts/setup.sh
for ((i=1;i<=$servers;i+=1)); do
	s=`printf "%02d" $i`;
	echo net$s
	echo "ssh to net$s and run ./ragam_server"
	ssh net$s "cd $ROOT_DIR; nohup ./ragam_server > output/s_$s 2>&1 &"
done
for ((;i<=$total;i+=1)); do
	s=`printf "%02d" $i`;
	echo net$s
	echo "ssh to net$s and run ./ragam_client"
	ssh net$s "cd $ROOT_DIR; nohup ./ragam_client > output/c_$s 2>&1 &"
done
