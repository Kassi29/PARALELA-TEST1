#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void multiply_row(int *A, int *B, int row, int N, int *result) {
    for (int j = 0; j < N; j++) {
        result[j] = 0;
        for (int k = 0; k < N; k++) {
            result[j] += A[row * N + k] * B[k * N + j];
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0) {
            printf("Este programa requiere exactamente 3 procesos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        printf("Ingrese el tamaño de las matrices (N): ");
        scanf("%d", &N);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *A = (int *)malloc(N * N * sizeof(int));
    int *B = (int *)malloc(N * N * sizeof(int));
    int *C = (int *)malloc(N * N * sizeof(int));

    if (rank == 0) {
        printf("Ingrese la matriz A (%d x %d):\n", N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%d", &A[i * N + j]);
            }
        }

        printf("Ingrese la matriz B (%d x %d):\n", N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%d", &B[i * N + j]);
            }
        }
    }

    MPI_Bcast(A, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int result[N];
        multiply_row(A, B, 0, N, result);
        MPI_Send(result, N, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }

    else if (rank == 1) {
        int result[N];
        multiply_row(A, B, 1, N, result);
        MPI_Send(result, N, MPI_INT, 2, 1, MPI_COMM_WORLD);
    }

    else if (rank == 2) {
        int fila1[N], fila2[N], fila3[N];
        MPI_Recv(fila1, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(fila2, N, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        multiply_row(A, B, 2, N, fila3);

        for (int j = 0; j < N; j++) {
            C[0 * N + j] = fila1[j];
            C[1 * N + j] = fila2[j];
            C[2 * N + j] = fila3[j];
        }

        printf("Resultado de la multiplicación de matrices:\n");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i * N + j]);
            }
            printf("\n");
        }
    }

    free(A);
    free(B);
    free(C);
    MPI_Finalize();
    return 0;
}
