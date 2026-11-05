#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[4][4] = {
        {1, 2, 2, 3},
        {2, 3, 4, 4},
        {1, 4, 5, 5},
        {0, 2, 5, 9}
    };
    int local[10] = {0}, global[10] = {0};
    for(int i = 0; i < size; i++){
        local[data[rank][i]]++;
    }

    MPI_Allreduce();

    if(rank == 0){
        printf("Final histogram after MPI_Allreduce:\nValue      Frequency\n");
        for(int i = 0; i < 10; i++){
            printf("%d        %d\n", i, global[i]);
        }
    }

    MPI_Finalize();
    return 0;
}