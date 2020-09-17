#include <stdio.h>
#include <string.h>
#include "mpi.h"


int main(int argc, char** argv)
{
    int my_rank; // Process id.
    int num_processes; // Number of processes.
    int tag = 50; // Message tag.

    char message_buffer[100];
    MPI_Status status; // Return status.

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    if (my_rank != 0)
    {
        sprintf(message_buffer, "Greetings from process %d!", my_rank);
        int recipient = 0;
        MPI_Send(message_buffer, strlen(message_buffer) + 1, MPI_CHAR, recipient, tag, MPI_COMM_WORLD);
    }
    else
    {
        for (int source = 1; source < num_processes; source++) {
            MPI_Recv(message_buffer, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
            printf("%s\n", message_buffer);
        }
    }
    MPI_Finalize();
    return 0;
}
