CC=g++
CFLAGS=

default:
	@echo "Options to run:\n\
	- hello\n\
	- string\n\
	- math\n\
	- input\n\
	- array\n\
	- pointers\n\
	"

hello:
	$(CC) hello_world.cpp -o hello_world.o $(CFLAGS)
	./hello_world.o

string:
	$(CC) string.cpp -o string.o $(CFLAGS)
	./string.o

math:
	$(CC) math.cpp -o math.o $(CFLAGS)
	./math.o

input:
	$(CC) input.cpp -o input.o $(CFLAGS)
	./input.o

array:
	$(CC) array.cpp -o array.o $(CFLAGS)
	./array.o

pointers:
	$(CC) pointers.cpp -o pointers.o $(CFLAGS)
	./pointers.o

.PHONY : clean
clean:
	rm -rf *.o