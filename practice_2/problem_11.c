#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]){
    MPI_Init(&argc ,&argv);
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &size);

    int parameter[3];
    if(rank == 0){
        parameter[0] = 1000;
        parameter[1] = 50;
        parameter[2] = 256;
    }
    MPI_Bcast(&parameter, 3, MPI_INT, 0, MPI_COMM_WORLD);
    printf("P%d receive: iteration: %d Threshold: %d Block: %d\n", rank, parameter[0], parameter[1], parameter[2]);
    MPI_Finalize();
    return 0;
}