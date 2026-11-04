#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size, data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int items[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    if(rank == 0){
        for(int i = 0; i < 8; i++){
            int consumer = ((i % 2) == 0) ? 1 : 2;
            MPI_Send(&items[i], 1, MPI_INT, consumer, 0, MPI_COMM_WORLD);
            printf("P0 produce: %d\n", items[i]);
        }
        int brk = -1;
        MPI_Send(&brk, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&brk, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }else{
        while(1){
            MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(data == -1)
                break;
            printf("P%d consume: %d\n", rank, data);
        }
        printf("All consumed.");
    }
    MPI_Finalize();
    return 0;
}