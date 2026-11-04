#include <stdio.h>
#include <mpi.h>

#define N 8

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with 2 processes!\n");

        MPI_Finalize();
        return 0;
    }

    int A[N], B[N];

    // Initialize array
    for (int i = 0; i < N; i++)
        A[i] = (rank == 0) ? (i + 1) * 10 : i + 1;

    double start, end;
    double blocking_time, nonblocking_time;

    // Blocking communication
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Send(A, N, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD);

    MPI_Recv(B, N, MPI_INT, 1 - rank, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    end = MPI_Wtime();
    blocking_time = end - start;

    // Non-blocking communication
    MPI_Request req[2];

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Irecv(B, N, MPI_INT, 1 - rank, 1,
              MPI_COMM_WORLD, &req[0]);

    MPI_Isend(A, N, MPI_INT, 1 - rank, 1,
              MPI_COMM_WORLD, &req[1]);

    MPI_Waitall(2, req, MPI_STATUSES_IGNORE);

    end = MPI_Wtime();
    nonblocking_time = end - start;

    // Print received array
    printf("P%d received: ", rank);

    for (int i = 0; i < N; i++)
        printf("%d ", B[i]);

    printf("\n");

    if (rank == 0) {
        printf("\nNonblocking communication completed.\n");
        printf("Blocking time    = %f seconds\n", blocking_time);
        printf("Nonblocking time = %f seconds\n", nonblocking_time);
    }

    MPI_Finalize();
    return 0;
}