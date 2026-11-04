#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = (rank + 1) * 10;
    int prefixSum;
    MPI_Scan(&value, &prefixSum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("MPI_Scan - P%d: local value: %d, Prefix sum: %d\n", rank, value, prefixSum);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        printf("Now implement manually using MPI_Send and MPI_Recv\n");
    }

    if(rank < size - 1){
        MPI_Send(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
    }else{
        int total = value, num;
        printf("Manullay - P%d: local value: %d, Prefix sum: %d\n", rank, value, total);
        for(int i = 0; i < size - 1; i++){
            MPI_Recv(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += num;
            printf("Manullay - P%d: local value: %d, Prefix sum: %d\n", i, num, total);
        }
        printf("Total sum: %d\n", total);
    }

    MPI_Finalize();
    return 0;
}