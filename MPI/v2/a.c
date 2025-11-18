// mpicc a.c -o a 
// mpirun -np 10 -oversubscribe ./a 12 uerjjjj

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void escreve_no_arquivo(int rank, int valor, const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "a");  // Abre o arquivo em modo de append

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return;
    }

    fprintf(arquivo, "Identificacao = %d valor %d\n", rank, valor);

    fclose(arquivo);
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Entre com o valor inicial e o nome do arquivo de saida\n");
        return 1;
    }

    int numTasks, rank, dest, source, count, tag = 1;
    int inmsg, outmsg;

    MPI_Status Stat;
    const char *nome_arquivo = argv[2];  // Nome do arquivo passado como argumento

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int valor_inicial = atoi(argv[1]);  // O primeiro argumento é o valor inicial
    for (int i = 0; i < numTasks; ++i) {
        if (i == 0 && rank == 0) {
            dest = rank + 1;
            valor_inicial += rank;
            outmsg = valor_inicial;
            escreve_no_arquivo(rank, outmsg, nome_arquivo);
            MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
        else if (rank == i) {
            source = rank - 1;
            dest = rank + 1;
            MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
            inmsg += rank;
            outmsg = inmsg;
            escreve_no_arquivo(rank, outmsg, nome_arquivo);
            if (i != numTasks - 1)
                MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
