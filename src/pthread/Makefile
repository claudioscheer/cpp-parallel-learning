CC=g++
CFLAGS=-lpthread

default:
	@echo "Options to run:\n\
		- create_threads\n\
		"

create_threads:
	$(CC) create_threads.cpp -o create_threads.o $(CFLAGS)
	./create_threads.o

.PHONY : clean
clean:
	rm -rf *.o
