#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

// mpicc tres.c -o ./tres
// mpirun -n 4 ./tres 100 10

/*
Escreva um programa para medir o tempo gasto para enviar um vetor (tamanhos 100, 1K e 1M)
de números de precisão dupla (double) do processo zero para o processo 1 usando MPI_Send
e MPI_Recv. Cada vetor deve ser armazenado dinamicamente e iniciado com o valor -1.0.
O processo zero deve localmente trocar este valor para 2.0 e o processo um para o valor 4.0.
O tamanho do vetor e a quantidade de trocas devem ser passados como parâmetros.

params: tam qtd_trocas
*/

int main(int argc, char **argv)
{
    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    int numTasks, rank;
    int tag = 1;

    const int TAMANHO_VEC = atoi(argv[1]);  // 1o param cmd
    const int QTD_TROCA = atoi(argv[2]); // 2o param cmd

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double *vetor = (double *)malloc(TAMANHO_VEC * sizeof(double));

    for (int i = 0; i < TAMANHO_VEC; i++)
        vetor[i] = -1.0;

    MPI_Status status;

    for (int i = 0; i < QTD_TROCA; i++)
    {
        if (rank == 0) // trocando p 2
        {
            for (int j = 0; j < TAMANHO_VEC; j++)
                vetor[j] = 2.0;

            MPI_Send(vetor, TAMANHO_VEC, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD);
            MPI_Recv(vetor, TAMANHO_VEC, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, &status);
        }
        else if (rank == 1) // trocando p 4
        {
            MPI_Recv(vetor, TAMANHO_VEC, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

            for (int j = 0; j < TAMANHO_VEC; j++)
                vetor[j] = 4.0;

            MPI_Send(vetor, TAMANHO_VEC, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        }
    }

    free(vetor);
    MPI_Finalize();

    gettimeofday(&fim, NULL);

    if (rank == 0)
    {
        double mili = (double)(fim.tv_sec - inicio.tv_sec) * 1000.0 + (double)(fim.tv_usec - inicio.tv_usec) / 1000.0;

        printf("Tamanho do vetor: %d e quantidade de trocas: %d\n", TAMANHO_VEC, QTD_TROCA);
        printf("Tempo total: %.3f segundos\n", mili/1000); // se for em milissegundos tirar o /1000
    }

    return 0;
}
