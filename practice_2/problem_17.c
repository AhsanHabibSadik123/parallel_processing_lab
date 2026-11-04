#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data[size], recv_data[size];

    for(int i = 0; i < size; i++){
        send_data[i] = (rank * 10) + i;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    MPI_Alltoall(&send_data, 1, MPI_INT, &recv_data, 1, MPI_INT, MPI_COMM_WORLD);
    
    double end = MPI_Wtime();
    
    printf("P%d Receive: ", rank);

    for(int i = 0; i < size; i++){
        printf("%d ", recv_data[i]);
    }

    printf("\n\nP%d Communication time: %f\n",rank, (end - start));
    MPI_Finalize();
    return 0;
}