#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);

    int value, rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    
    if(rank == 0){
        scanf("%d", &value);
        printf("Value is %d\n", value);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("P0 send %d to P%d\n", value, next);

        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 Receive %d from P%d\n", value, prev);
    }
    else{

        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d Receive %d from P%d\n", rank, value, prev);

        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("P%d send %d to P%d\n",rank, value, next);
    }
    MPI_Finalize();
    return 0;
}