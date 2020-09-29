#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <vector>

int main(int argc, char **argv) {
    int my_rank;       // Process id.
    int num_processes; // Number of processes.
    int tag = 1;
    int vector_size = 7;

    MPI_Status status; // Return status.
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    std::vector<std::vector<int>> values;
    for (int i = 1, j = 0; i <= num_processes; i++) {
        std::vector<int> v;
        for (int z = 0; z < vector_size; z++) {
            v.push_back(j);
            j++;
        }
        values.push_back(v);
    }

    if (my_rank != 0) {
        int master = 0;
        std::vector<int> v(vector_size);
        MPI_Recv(&v[0], vector_size, MPI_INT, master, tag, MPI_COMM_WORLD,
                 &status);
        printf("%d - %d\n", my_rank, (int)v.size());
        /* for (int i = 0; i < v.size(); i++) { */
        /*     /1* v[i] = v[i]; *1/ */
        /* } */
        /* MPI_Send(&v, vector_size, MPI_INT, master, tag, MPI_COMM_WORLD); */
    } else {
        double start_time = MPI_Wtime();
        for (int i = 1; i <= num_processes; i++) {
            std::vector<int> v = values.at(i - 1);
            MPI_Send(&v[0], vector_size, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
        for (int i = 1; i <= num_processes; i++) {
            std::vector<int> result(vector_size);
            MPI_Recv(&result[0], vector_size, MPI_INT, i, tag, MPI_COMM_WORLD,
                     &status);
            printf("%d - %d\n", i, result[0]);
        }
        double end_time = MPI_Wtime();
        printf("Execution time: %f seconds\n", end_time - start_time);
    }
    MPI_Finalize();
    return 0;
}
