#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0)
            cout << "Run with 3 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int items[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int data, consumer;

    if (rank == 0) {  // Producer
        for (int i = 0; i < 8; i++) {
            consumer = (i % 2 == 0) ? 1 : 2;

            cout << "P0 produced: " << items[i] << endl;
            MPI_Send(&items[i], 1, MPI_INT, consumer, 0,
                     MPI_COMM_WORLD);
        }

        data = -1;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } else {  // Consumers
        while (true) {
            MPI_Recv(&data, 1, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (data == -1)
                break;

            cout << "P" << rank << " consumed: "
                 << data << endl;
        }
    }

    if (rank == 0)
        cout << "All 8 items processed successfully." << endl;

    MPI_Finalize();
    return 0;
}