CC=g++
CFLAGS=-ltbb

default:
	@echo "Options to run:\n\
	- parallel_for\n\
	"

parallel_for:
	$(CC) parallel_for.cpp -o parallel_for.o $(CFLAGS)
	./parallel_for.o

.PHONY : clean
clean:
	rm -rf *.o