#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            cout << "Run with 4 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int data[4][4] = {
        {1, 2, 2, 3},
        {2, 3, 4, 4},
        {1, 4, 5, 5},
        {0, 2, 5, 9}
    };

    int local[10] = {0};
    int global[10] = {0};

    // Calculate local histogram
    for (int i = 0; i < 4; i++)
        local[data[rank][i]]++;

    // Combine all local histograms
    MPI_Allreduce(local, global, 10, MPI_INT,
                  MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Final histogram after MPI_Allreduce:\n";
        cout << "Value    Frequency\n";

        for (int i = 0; i < 10; i++)
            if (global[i] > 0)
                cout << i << "        " << global[i] << endl;
    }

    MPI_Finalize();
    return 0;
}