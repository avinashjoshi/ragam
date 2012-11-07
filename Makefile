CC := gcc
OBJECTS := client_src/main.o client_src/helper.o client_src/tcp.o client_src/compute.o client_src/handle_receive.o client_src/r_queue.o client_src/d_queue.o
SERVER_OBJ := server_src/main.o server_src/helper.o server_src/tcp.o server_src/handle_receive.o server_src/r_queue.o

#%.o: %.c
#	$(CC) -c -o $@ $< $(LDFLAGS)

#Flags
CFLAGS := -Wall
LDFLAGS := -pthread
LDLIBS :=

default: client server

all: client server

client: $(OBJECTS)
	gcc -o ragam_client $(OBJECTS) $(LDFLAGS)

server: $(SERVER_OBJ)
	gcc -o ragam_server $(SERVER_OBJ) $(LDFLAGS)

clobber: clean
	-rm -f ragam_client ragam_server

clean:
	-rm -f core $(OBJECTS) $(SERVER_OBJ)

help:
	@echo ""
	@echo "make		- builds ragam"
	@echo "make clean	- remove *.o files"
	@echo "make clobber	- removes all generated files"
	@echo "make help	- this info"
	@echo ""
