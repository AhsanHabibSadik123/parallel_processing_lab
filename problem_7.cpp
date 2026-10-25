#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 8;
    int A[N], B[N];

    if (size != 2) {
        if (rank == 0)
            cout << "Run with 2 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    for (int i = 0; i < N; i++)
        A[i] = rank == 0 ? (i + 1) * 10 : i + 1;

    double start, end;

    // Blocking communication
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Send(A, N, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD);
    MPI_Recv(B, N, MPI_INT, 1 - rank, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    end = MPI_Wtime();
    double blocking_time = end - start;

    // Nonblocking communication
    MPI_Request req[2];

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Irecv(B, N, MPI_INT, 1 - rank, 1, MPI_COMM_WORLD, &req[0]);
    MPI_Isend(A, N, MPI_INT, 1 - rank, 1, MPI_COMM_WORLD, &req[1]);

    MPI_Waitall(2, req, MPI_STATUSES_IGNORE);

    end = MPI_Wtime();
    double nonblocking_time = end - start;

    cout << "P" << rank << " received: ";
    for (int i = 0; i < N; i++)
        cout << B[i] << " ";
    cout << endl;

    if (rank == 0) {
        cout << "Nonblocking communication completed." << endl;
        cout << "Blocking communication time    = "
             << blocking_time << " s" << endl;
        cout << "Nonblocking communication time = "
             << nonblocking_time << " s" << endl;
    }

    MPI_Finalize();
    return 0;
}