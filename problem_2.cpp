#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    int row[4];
    int sum;

    if (rank == 0)
    {
        // Root keeps first row
        for (int i = 0; i < 4; i++)
            row[i] = matrix[0][i];

        sum = row[0] + row[1] + row[2] + row[3];

        cout << "P0: Row = ";
        for (int i = 0; i < 4; i++)
            cout << row[i] << " ";
        cout << "\tSum = " << sum << endl;

        // Send rows to workers
        for (int p = 1; p < size; p++)
        {
            MPI_Send(matrix[p], 4, MPI_INT, p, 0, MPI_COMM_WORLD);
        }

        // Receive sums
        int total = sum;
        int workerSum;

        for (int p = 1; p < size; p++)
        {
            MPI_Recv(&workerSum, 1, MPI_INT, p, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            total += workerSum;
        }

        cout << "\nTotal matrix sum = " << total << endl;
    }
    else
    {
        // Receive row
        MPI_Recv(row, 4, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate sum
        sum = row[0] + row[1] + row[2] + row[3];

        cout << "P" << rank << ": Row = ";

        for (int i = 0; i < 4; i++)
            cout << row[i] << " ";

        cout << "\tSum = " << sum << endl;

        // Send sum to root
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}