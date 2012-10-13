CC := gcc
OBJECTS := main.o helper.o tcp.o compute.o handle_receive.o r_queue.o d_queue.o

#%.o: %.c
#	$(CC) -c -o $@ $< $(LDFLAGS)

#Flags
CFLAGS := -Wall
LDFLAGS := -pthread
LDLIBS :=

ragam: $(OBJECTS)
	gcc -o ragam $(OBJECTS) $(LDFLAGS)

#main.o helper.o: my_header.h

clobber: clean
	-rm -f ragam

clean:
	-rm -f core $(OBJECTS)

help:
	@echo ""
	@echo "make		- builds ragam"
	@echo "make clean	- remove *.o files"
	@echo "make clobber	- removes all generated files"
	@echo "make help	- this info"
	@echo ""
