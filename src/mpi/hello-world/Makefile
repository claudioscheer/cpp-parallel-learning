clean:
	@rm -f hello_mpi.o
	@rm -f hello_messages.o
	@rm -f hello_int_vector.o

hello_mpi:
	mpic++ hello_mpi.cpp -o hello_mpi.o
	mpirun ./hello_mpi.o

hello_messages:
	mpic++ hello_messages.cpp -o hello_messages.o
	mpirun -np 6 ./hello_messages.o

hello_int_vector:
	mpic++ hello_int_vector.cpp -o hello_int_vector.o
	mpirun -np 6 ./hello_int_vector.o

.PHONY: all clean
