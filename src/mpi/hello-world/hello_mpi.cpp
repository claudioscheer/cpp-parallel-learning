#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char hostname[20];

    FILE *host;

    if ((host = fopen("/etc/hostname", "r")) == NULL) {
        printf("\nCannot open the file 'hostname'!");
        exit(1);
    }

    fscanf(host, "%s", hostname);

    int my_rank;       // Who am i?
    int num_processes; // How many process?

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    printf("Hostname: %s\nRank: %d\nSize: %d\n", hostname, my_rank,
           num_processes);
    MPI_Finalize();
    return 0;
}
