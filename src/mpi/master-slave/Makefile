clean:
	@rm -f square_value.o
	@rm -f square_vector.o

square_value:
	mpic++ square_value.cpp -o square_value.o
	mpirun -np 16 --oversubscribe ./square_value.o

square_vector:
	mpic++ square_vector.cpp -o square_vector.o
	mpirun -np 2 --oversubscribe ./square_vector.o

.PHONY: all clean
