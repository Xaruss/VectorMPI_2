#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Create a ring topology for 4 processes
    int dims[1] = {4};
    int periods[1] = {1};
    MPI_Comm ring_comm;
    MPI_Dims_create(size, 1, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, 0, &ring_comm);

    // Initialize the vector
    int n = 10;
    float vector[n];
    for (int i = 0; i < n; i++)
        vector[i] = 1.0;

    // Calculate the sum of vector elements
    float local_sum = 0.0;
    for (int i = 0; i < n; i++)
        local_sum += vector[i];

    // Send the sum to neighboring processes
    int left, right;
    MPI_Cart_shift(ring_comm, 0, 1, &left, &right);
    MPI_Sendrecv(&local_sum, 1, MPI_FLOAT, right, 0, &local_sum, 1, MPI_FLOAT, left, 0, ring_comm, MPI_STATUS_IGNORE);

    // Print the result
    if (rank == 0)
        printf("Sum of vector elements: %f\n", local_sum);

    MPI_Finalize();
    return 0;
}
