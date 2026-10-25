#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *send_data = new int[size];
    int *recv_data = new int[size];

    for (int i = 0; i < size; i++)
        send_data[i] = rank * 10 + i;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    MPI_Alltoall(send_data, 1, MPI_INT,
                 recv_data, 1, MPI_INT, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    cout << "P" << rank << " received: ";
    for (int i = 0; i < size; i++)
        cout << recv_data[i] << " ";
    cout << endl;

    cout << "P" << rank << " communication time = "
         << end - start << " s" << endl;

    delete[] send_data;
    delete[] recv_data;

    MPI_Finalize();
    return 0;
}