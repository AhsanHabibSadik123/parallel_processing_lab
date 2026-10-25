#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = (rank + 1) * 10;
    int prefix;

    // Method 1: MPI_Scan
    MPI_Scan(&value, &prefix, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout << "P" << rank << ": Local value = " << value
         << ", Prefix sum = " << prefix << endl;

    // Method 2: Manual point-to-point
    int manual = value, temp;

    if (rank > 0) {
        MPI_Recv(&temp, 1, MPI_INT, rank - 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        manual += temp;
    }

    if (rank < size - 1)
        MPI_Send(&manual, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    if (rank == size - 1) {
        cout << "Manual prefix result = " << manual << endl;
    }

    MPI_Finalize();
    return 0;
}