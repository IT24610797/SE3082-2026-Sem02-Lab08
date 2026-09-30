#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000LL

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long chunk = N / size;
    long long start = rank * chunk + 1;       
    long long end   = (rank == size - 1) ? N : start + chunk - 1;

    double t0 = MPI_Wtime();

    long long local_sum = 0;
    for (long long i = start; i <= end; i++)
        local_sum += i;

    long long total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double t1 = MPI_Wtime();

    if (rank == 0) {
        long long expected = N * (N + 1) / 2;
        printf("N = %lld\n", N);
        printf("Total = %lld\n", total_sum);
        printf("Expected = %lld\n", expected);
        printf("Correct? = %s\n", total_sum == expected ? "YES" : "NO");
        printf("Procs = %d\n", size);
        printf("Time = %.6f sec\n", t1 - t0);
    }

    MPI_Finalize();
    return 0;
}