clean:
	@rm -f pipeline.o

pipeline:
	mpic++ pipeline.cpp -o pipeline.o
	mpirun -np 4 ./pipeline.o

.PHONY: all clean
