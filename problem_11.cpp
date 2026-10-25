#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int parameters[3];

    if (rank == 0) {
        parameters[0] = 1000;  // Iterations
        parameters[1] = 50;    // Threshold
        parameters[2] = 256;   // Block size
    }

    MPI_Bcast(parameters, 3, MPI_INT, 0, MPI_COMM_WORLD);

    cout << "P" << rank << " received: Iterations = "
         << parameters[0] << ", Threshold = "
         << parameters[1] << ", Block size = "
         << parameters[2] << endl;

    MPI_Finalize();
    return 0;
}