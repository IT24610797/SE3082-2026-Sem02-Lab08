#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL 10000000LL

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long per_rank = TOTAL / size;

    srand(12345 + rank);

    double t0 = MPI_Wtime();

    long long inside = 0;
    for (long long i = 0; i < per_rank; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
            inside++;
    }

    double t1 = MPI_Wtime();

    if (rank != 0) {
        MPI_Send(&inside, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total_inside = inside;   

        for (int i = 1; i < size; i++) {
            long long recv_count;
            MPI_Status status;

            MPI_Recv(&recv_count, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            printf("Root received %lld from rank %d\n",
                   recv_count, status.MPI_SOURCE);

            total_inside += recv_count;
        }

        double pi = 4.0 * (double)total_inside / (double)TOTAL;
        printf("\nSamples  = %lld\n", TOTAL);
        printf("Inside   = %lld\n", total_inside);
        printf("Pi est.  = %.8f\n", pi);
        printf("Error    = %.8f\n", pi - 3.14159265358979);
        printf("Procs    = %d\n", size);
        printf("Time     = %.6f sec\n", t1 - t0);
    }

    MPI_Finalize();
    return 0;
}