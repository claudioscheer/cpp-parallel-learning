#include <stdio.h>
#include <string.h>
#include "mpi.h"


int main(int argc, char** argv)
{
    int my_rank; // Process id.
    int num_processes; // Number of processes.
    int tag = 1; // Message tag.

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    if (my_rank != 0)
    {
        int data[] = { 0, 1, 2, 3, 4, 5, 6 };
        int recipient = 0;
        MPI_Send(data, 7, MPI_INT, recipient, tag, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status status; // Return status.
        int data[7];
        for (int source = 1; source < num_processes; source++) {
            MPI_Recv(data, 7, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            for (int i = 0; i < 7; i++) {
                printf("%d", data[i]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}
