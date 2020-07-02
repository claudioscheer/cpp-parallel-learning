CC=g++
CFLAGS=-I. -pthread

multithread:
	$(CC) multithread.cpp -o multithread.o $(CFLAGS)
	./multithread.o

multicore:
	$(CC) multicore.cpp -o multicore.o $(CFLAGS)
	./multicore.o

.PHONY : clean
clean:
	rm -rf *.o