#!/bin/sh
ROOT_DIR="/home/004/a/ax/axj107420/aos/ragam"
clients=`cat $ROOT_DIR/config/commons.h | grep MAX_CLIENTS | cut -f3 -d" "`
servers=`cat $ROOT_DIR/config/commons.h | grep MAX_SERVERS | cut -f3 -d" "`
total=`expr $clients + $servers`
echo $total
#echo "-------  :file:  ---- :stdout:"
for ((i=1;i<=$servers;i+=1)); do
	s=`printf "%02d" $i`;
	echo net$s
	echo "ssh to net$s and killing server"
	ssh -n -f net$s "pkill -9 server"
done
for ((;i<=$total;i+=1)); do
	s=`printf "%02d" $i`;
	echo net$s
	echo "ssh to net$s and killing client"
	ssh -n -f net$s "pkill -9 client"
done
