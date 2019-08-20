#include <mpi.h>
#include <stdio.h>
# include <unistd.h>
# include <omp.h>
# define STD_TAG 0

//Cozinheiro - Rank 0
//Gaulês - restante dos ranks
int main(int argc, char **argv){
  int rank, size, i, javali=18;
  char fome='f'; //char referente a "envelope" vazio

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(rank==0){ //Cozinheiro
    printf("Cozinheiro de rank %d\n", rank);
    while(1){
        printf("Cozinheiro colocando comida para gauleses\n");
        MPI_Send(&javali, sizeof(javali), MPI_INT, 1, STD_TAG, MPI_COMM_WORLD);
        MPI_Recv(&fome, sizeof(fome), MPI_CHAR, 1, STD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }//função bloqueante, logo só sai do for quando todos os gauleses estiverem com fome
    }
  else{
    int qntd_javali = 0;
    #pragma omp parallel
    {
      int gaules = omp_get_thread_num();
      printf("gaules %d\n", gaules);
      sleep(3);
      while(1){
        #pragma omp critical
        {
          if(qntd_javali == 0){
            MPI_Recv(&qntd_javali, sizeof(qntd_javali), MPI_INT, 0, STD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&fome, sizeof(fome), MPI_CHAR, 0, STD_TAG, MPI_COMM_WORLD);
          }
          printf("Gaules %d comendo javali, restam %d\n", gaules, --qntd_javali);
        }
      }
    }
  }
}
