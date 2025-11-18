// mpicc seg.c -o seg 
// mpirun -np 10 -oversubscribe ./seg nomedoarquivo

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_MESSAGENS 10

int main(int argc, char **argv)
{
    int numTasks, rank, dest, source, count, tag = 1;
    int inmsg, outmsg;

    MPI_Status Stat;
    const char *nome_arquivo = argv[1];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numero_messagens = 0;

    int max_messages = (numTasks - 1) * MAX_MESSAGENS;

    while (numero_messagens < max_messages)
    {
        if (rank == 0)
        {
            FILE *arquivo = fopen(argv[1], "a");

            MPI_Recv(&inmsg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
            fprintf(arquivo, "Mensagem = %d ,total de msgs: %d\n", inmsg, numero_messagens);
            ++numero_messagens;

            fclose(arquivo);
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
