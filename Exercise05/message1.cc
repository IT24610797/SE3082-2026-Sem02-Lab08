// mismatch.cc  -  rank 0 sends to rank 1
// mpicxx mismatch.cc -o mismatch && timeout 10 mpirun -np 3 ./mismatch
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = 0;

    if (rank == 0) {
        number = 42;
        std::cout << "Process 0 sending " << number << " to process 1\n";
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 finished send\n";
    } else if (rank == 1) {
        std::cout << "Process 1 waiting to receive from process 2\n";
        MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 1 received " << number << "\n";
    } else {
        std::cout << "Process " << rank << " idle (never sends anything)\n";
    }

    MPI_Finalize();
    return 0;
}