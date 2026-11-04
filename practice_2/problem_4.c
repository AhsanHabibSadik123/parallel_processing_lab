#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size, iteration = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sizes[] = {64, 256, 1024, 4096, 16384, 65536, 1048576};
    for (int i = 0; i < 7; i++)
    {
        int n = sizes[i];
        char *data = (char *)malloc(n * sizeof(char));
        double st, et;
        MPI_Barrier(MPI_COMM_WORLD);

        st = MPI_Wtime();
        for (int i = 0; i < iteration; i++){
            if (rank == 0){
                MPI_Send(data, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(data, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else{
                MPI_Recv(data, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(data, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
        et = MPI_Wtime();
        if (rank == 0){
            double round_trip = (et - st) / iteration;
            printf("\nMessage size       : %d bytes\n", n);
            printf("Iterations         : %d\n", iteration);
            printf("Average round-trip : %f seconds\n", round_trip);
            printf("Average one-way    : %f seconds\n", round_trip / 2);
        }
        free(data);
    }
    MPI_Finalize();
    return 0;
}