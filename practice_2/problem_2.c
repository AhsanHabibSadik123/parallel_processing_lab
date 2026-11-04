#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
# define N 512

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[N][N];
    int row[N];
    int rows, cols;
    if(rank == 0){
        FILE *file = fopen("input.txt", "r");
        fscanf(file, "%d %d", &rows, &cols);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                fscanf(file, "%d", &matrix[i][j]);
            }
        }
        fclose(file);
    }

    // share the rows and cols
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    

    int sum = 0;
    if(rank == 0){
        int total = 0;
        printf("P0: Row = ");
        for(int i = 0; i < cols; i++){
            total += matrix[0][i];
            printf("%d ", matrix[0][i]);
        }

        printf("sum = %d", total);
        sum += total;
        for(int i = 1; i < rows; i++){
            MPI_Send(matrix[i], cols, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int workerSum;
        for(int i = 1; i < rows; i++){
            MPI_Recv(&workerSum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
            sum += workerSum;
        }
        printf("\n Total matrix sum = %d", sum);
    }
    else{
        MPI_Recv(&row, cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int total = 0;
        printf("P%d: Row = ", rank);
        for(int i = 0; i < cols; i++){
            total += row[i];
            printf("%d ", row[i]);
        }
        printf("sum = %d", total);
        MPI_Send(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}