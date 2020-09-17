CC=g++
CFLAGS=-ltbb

default:
	@echo "Options to run:\n\
		- parallel_reduce\n\
		- parallel_map\n\
		"

parallel_reduce:
	$(CC) parallel_reduce.cpp -o parallel_reduce.o $(CFLAGS)
	./parallel_reduce.o

parallel_map:
	$(CC) parallel_map.cpp -o parallel_map.o $(CFLAGS)
	./parallel_map.o

.PHONY : clean
clean:
	rm -rf *.o
