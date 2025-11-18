
// mpicc a.c -o a 
// mpirun -np 10 -oversubscribe ./a 12 uerjjjj

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Entre com o valor inicial e o nome do arquivo de saida\n");
        return 1;
    }

    int numTasks, rank, dest, source, count, tag = 1;
    int inmsg, outmsg;
    int valor_inicial, i;

    MPI_Status Stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        valor_inicial = atoi(argv[1]); 
        FILE *arquivo = fopen(argv[2], "w");  // Abre o arquivo em modo de append
        if (arquivo == NULL) {
             fprintf(stderr, "Erro ao abrir o arquivo\n");
            return -1;
        }
        dest = rank+1;
        fprintf(arquivo, "Identificacao = %d valor %d\n", rank, valor_inicial);
        MPI_Send(&valor_inicial, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        for (i = 1; i < numTasks; ++i){
            MPI_Recv(&inmsg, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
            fprintf(arquivo, "Identificacao = %d valor %d\n", i, inmsg);

        }
        fclose(arquivo);
    }
    else{
        if (rank == numTasks-1){
            source = rank - 1;
            MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
            inmsg = rank+ inmsg;
             MPI_Send(&inmsg, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }
        else{
            dest = rank + 1;
            source = rank - 1;
            MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
            inmsg = rank+ inmsg;
            MPI_Send(&inmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send(&inmsg, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        }
        
    }

    MPI_Finalize();
    return 0;
}
