#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100
int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[N][N], B[N][N], C[N][N];
    int row[N], result[N];

    int r1 = 0, c1 = 0, r2 = 0, c2 = 0;

    if(rank == 0){
        FILE *file = fopen("input.txt", "r");
        fscanf(file, "%d %d", &r1, &c1);
        for(int i = 0; i < r1; i++){
            for(int j = 0; j < c1; j++){
                fscanf(file, "%d", &A[i][j]);
            }
        }

        fscanf(file, "%d %d", &r2, &c2);
        for(int i = 0; i < r2; i++){
            for(int j = 0; j < c2; j++){
                fscanf(file, "%d", &B[i][j]);
            }
        }
        fclose(file);
    }

    MPI_Bcast(&r1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r2, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < r2; i++) {
        MPI_Bcast(B[i], c2, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if(rank == 0){
        for(int j = 0; j < c2; j++){
            C[0][j] = 0;
            for(int k = 0; k < c1; k++){
                C[0][j] += A[0][k] * B[k][j];
            }
        }

        for(int i = 1; i < r1; i++){
            MPI_Send(A[i], c1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for(int i = 1; i < r1; i++){
            MPI_Recv(C[i], c2, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

    }else{
        MPI_Recv(row, c1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int j = 0; j < c2; j++){
            result[j] = 0;
            for(int k = 0; k < c1; k++){
                result[j] += row[k] * B[k][j];
            }
        }
        MPI_Send(result, c2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if(rank == 0){
        printf("Result: \n");
        for(int i = 0; i < r1; i++){
            for(int j = 0; j < c2; j++){
                printf("%d ", C[i][j]);
            }
            printf("\n");
        } 
    }
    MPI_Finalize();
    return 0;
}
