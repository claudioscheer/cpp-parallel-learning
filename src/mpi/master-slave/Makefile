clean:
	@rm -f square_value.o

square_value:
	mpic++ square_value.cpp -o square_value.o
	mpirun -np 16 --oversubscribe ./square_value.o

.PHONY: all clean
