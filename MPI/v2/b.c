#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_MESSAGENS 100

void escreve_no_arquivo(int rank, int valor, const char *nome_arquivo);

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Entre com o nome do arquivo de saida\n");
        return 1;
    }

    int numTasks, rank, dest, source, count, tag = 1;
    int inmsg, outmsg;

    MPI_Status Stat;
    const char *nome_arquivo = argv[1];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numero_messagens = 0;
    int max_messages = MAX_MESSAGENS;

    if (rank == 0)
    {
        max_messages = (numTasks - 1) * MAX_MESSAGENS;
    }

    while (numero_messagens < max_messages)
    {
        if (rank == 0)
        {
            MPI_Recv(&inmsg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
            escreve_no_arquivo(inmsg, numero_messagens, nome_arquivo);
            ++numero_messagens;
        }
        else
        {
            dest = 0;
            outmsg = rank;
            MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            ++numero_messagens;
        }
    }

    MPI_Finalize();
    return 0;
}

void escreve_no_arquivo(int rank, int valor, const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "a");

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return;
    }

    fprintf(arquivo, "Mensagem = %d ,total de msgs: %d\n", rank, valor);

    fclose(arquivo);
}
