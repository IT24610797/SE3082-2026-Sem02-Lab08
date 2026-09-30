// message2.cc  -  rank 0 sends three numbers to rank 1 using the same variable
// mpicxx message2_bsend.cc -o message2_bsend && mpirun -np 2 ./message2_bsend
#include <mpi.h>
#include <iostream>
#include <cstdlib>

#define BUF_SIZE (1 * sizeof(int) + MPI_BSEND_OVERHEAD)

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Every rank that calls BSend must attach a buffer
    char* buf = (char*)malloc(BUF_SIZE);
    MPI_Buffer_attach(buf, BUF_SIZE);

    int number = -1;

    for (int i = 0; i < 3; i++) {
        if (rank == 0) {
            number = i * 10;
            std::cout << "Process 0 BSending " << number << "\n";
            MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        } else if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << "\n";
        }
    }

    int size_attached;
    MPI_Buffer_detach(&buf, &size_attached);
    free(buf);

    MPI_Finalize();
    return 0;
}