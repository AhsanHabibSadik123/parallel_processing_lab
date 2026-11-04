// Problem_8: Create a two-process communication program that can cause deadlock when blocking 
// sends are used. Modify the program to avoid deadlock using appropriate MPI communication 
// techniques. 
// Sample Input
// Number of processes: 2
// P0 message = 100
// P1 message = 200
// Sample Output
// Unsafe version:
// P0: Waiting to send 100
// P1: Waiting to send 200
// Potential deadlock detected.
// Corrected version:
// P0 sent 100
// P1 received 100
// P1 sent 200
// P0 received 200
// Communication completed without deadlock.

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    int send_data, recv_data;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with 2 processes!\n");

        MPI_Finalize();
        return 0;
    }

    send_data = (rank == 0) ? 100 : 200;

    // Unsafe version: Blocking send first
    if (rank == 0)
        printf("P0: Waiting to send 100\n");
    else
        printf("P1: Waiting to send 200\n");

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Send(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else {
        MPI_Send(&send_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank == 0)
        printf("Unsafe communication completed (if no deadlock occurred).\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // Corrected version: MPI_Sendrecv
    MPI_Sendrecv(&send_data, 1, MPI_INT, 1 - rank, 1,
                 &recv_data, 1, MPI_INT, 1 - rank, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("P%d received %d\n", rank, recv_data);

    if (rank == 0)
        printf("Communication completed without deadlock.\n");

    MPI_Finalize();
    return 0;
}