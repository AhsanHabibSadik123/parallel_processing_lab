#include<stdio.h>
#include<mpi.h>

double f(double x){
    return x * x;
}

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a = 0, b = 1;
    int n = 1000;
    double localSum = 0, total_sum = 0;

    double h = (double)(b - a) / n;

    for(int i = rank; i < n; i += size){
        double x1 = a + i * h;
        double x2 = a + (i + 1) * h;
        localSum += (f(x1) + f(x2)) * (h / 2.0);
    }
    printf("Local sum for p%d = %f\n", rank, localSum);
    MPI_Reduce(&localSum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0){
        printf("Total sum = %f", total_sum);
    }
    MPI_Finalize();
    return 0;
}
