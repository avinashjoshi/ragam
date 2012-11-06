CC := gcc
OBJECTS := client_src/main.o client_src/helper.o client_src/tcp.o client_src/compute.o client_src/handle_receive.o client_src/r_queue.o client_src/d_queue.o
SERVER_OBJ := server_src/main.o

#%.o: %.c
#	$(CC) -c -o $@ $< $(LDFLAGS)

#Flags
CFLAGS := -Wall
LDFLAGS := -pthread
LDLIBS :=

default: ragam server

client: ragam

all: ragam

ragam: $(OBJECTS)
	gcc -o ragam $(OBJECTS) $(LDFLAGS)

server: $(SERVER_OBJ)
	gcc -o server $(SERVER_OBJ) $(LDFLAGS)

#main.o helper.o: my_header.h

clobber: clean
	-rm -f ragam server

clean:
	-rm -f core $(OBJECTS) $(SERVER_OBJ)

help:
	@echo ""
	@echo "make		- builds ragam"
	@echo "make clean	- remove *.o files"
	@echo "make clobber	- removes all generated files"
	@echo "make help	- this info"
	@echo ""
