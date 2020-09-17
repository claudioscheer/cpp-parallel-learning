#include "mpi.h"
#include <stdio.h>
#include <string.h>

void generator(int size, int recipient, int tag_data, int tag_time) {
    double init_time[size];

    for (int i = 1; i <= size; i++) {
        int data[2];
        data[0] = i - 1; // Message id.
        data[1] = i;     // Data to be processed.

        init_time[data[0]] = MPI_Wtime();

        MPI_Send(&data, 2, MPI_INT, recipient, tag_data, MPI_COMM_WORLD);
    }

    MPI_Status status; // Return status.
    for (int i = 1; i <= size; i++) {
        int message_id;
        MPI_Recv(&message_id, 1, MPI_INT, MPI_ANY_SOURCE, tag_time,
                 MPI_COMM_WORLD, &status);

        double end_time = MPI_Wtime();
        double total_time = end_time - init_time[message_id];

        printf("%d - %f seconds.\n", message_id, total_time);
    }
}

void process_1(int size, int source, int recipient, int tag_data) {
    MPI_Status status; // Return status.
    int data[2];
    for (int i = 1; i <= size; i++) {
        MPI_Recv(&data, 2, MPI_INT, MPI_ANY_SOURCE, tag_data, MPI_COMM_WORLD,
                 &status);

        int data_temp = data[1] * data[1];
        data[1] = data_temp;

        MPI_Send(&data, 2, MPI_INT, recipient, tag_data, MPI_COMM_WORLD);
    }
}

void process_2(int size, int source, int recipient, int tag_data) {
    MPI_Status status; // Return status.
    int data[2];
    for (int i = 1; i <= size; i++) {
        MPI_Recv(&data, 2, MPI_INT, MPI_ANY_SOURCE, tag_data, MPI_COMM_WORLD,
                 &status);

        int data_temp = data[1] + 1;
        data[1] = data_temp;

        MPI_Send(&data, 2, MPI_INT, recipient, tag_data, MPI_COMM_WORLD);
    }
}

void printer(int size, int recipient, int source, int tag_data, int tag_time) {
    MPI_Status status; // Return status.
    int data[2];
    for (int i = 1; i <= size; i++) {
        MPI_Recv(&data, 2, MPI_INT, MPI_ANY_SOURCE, tag_data, MPI_COMM_WORLD,
                 &status);
        MPI_Send(&data[0], 1, MPI_INT, recipient, tag_time, MPI_COMM_WORLD);

        printf("%d - %d\n", data[0], data[1]);
    }
}

int main(int argc, char **argv) {
    int my_rank;       // Process id.
    int num_processes; // Number of processes.
    int tag_data = 1;
    int tag_time = 2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    int size = 7;
    int source = my_rank - 1;
    int recipient = my_rank + 1;
    switch (my_rank) {
    case 0:
        generator(size, recipient, tag_data, tag_time);
        break;

    case 1:
        process_1(size, source, recipient, tag_data);
        break;

    case 2:
        process_2(size, source, recipient, tag_data);
        break;

    case 3:
        printer(size, 0, source, tag_data, tag_time);
        break;
    }
    MPI_Finalize();
    return 0;
}
