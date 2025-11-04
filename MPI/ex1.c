#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int dado;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int valor; 

    if (rank == 0) {   // p0 lê o dado inicial
        if (argc < 2) {
            printf("Uso: mpirun -np <num_processos> ./programa <valor_inicial>\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        dado = atoi(argv[1]);
        valor = dado;

        if (size > 1)  // envia valor inicia para o p1
            MPI_Send(&valor, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(&valor, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    // recebendo o valor
        valor += rank; // soma rank ao valor

        if (rank < size - 1) MPI_Send(&valor, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); //envia pro prox caso nao esteja no ultimo
    }

    MPI_Send(&valor, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); // todos enviam o valor final ao p0

    if (rank == 0) { // p0 coleta resultados
        FILE *arq = fopen("valores.txt", "w");
        if (!arq) {
            printf("Erro ao abrir o arquivo!!.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fprintf(arq, "Identificação = %d valor %d\n", 0, dado);

        for (int i = 1; i < size; i++) {
            MPI_Recv(&valor, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(arq, "Identificação = %d valor %d\n", i, valor); // escrita
        }

        fclose(arq);
        printf("Gravado em valores.txt\n");
    }

    MPI_Finalize();
    return 0;
}
