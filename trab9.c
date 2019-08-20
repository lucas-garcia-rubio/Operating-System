#include <mpi.h>
#include <stdio.h>
# include <unistd.h>
# define STD_TAG 0

//Cozinheiro - Rank 0
//Gaulês - restante dos ranks
int main(int argc, char **argv){
  int rank, size, i, javali=5;
  char fome='f'; //char referente a "envelope" vazio

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(rank==0){ //Cozinheiro
    printf("Cozinheiro de rank %d\n", rank);
    sleep(2);
    while(1){
      for(i=1; i<size; i++){//for para mandar comida para cada gaules
        printf("Cozinheiro servindo 5 javalis para o gaules %d\n", i);
        //sleep(2);
        MPI_Send(&javali, sizeof(javali), MPI_INT, i, STD_TAG, MPI_COMM_WORLD);
      }

      for(i=1; i<size; i++){//for para receber as mensagens de cada gaulês
        MPI_Recv(&fome, sizeof(fome), MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }//função bloqueante, logo só sai do for quando todos os gauleses estiverem com fome
    }
  }
  else{//Gaulês
    printf("Gaules de rank %d\n", rank);
    int rank_gaules = rank;
    sleep(2);
    while(1){
      printf("rank_gaules antes do receive %d\n", rank_gaules);
      MPI_Recv(&javali, sizeof(javali), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("rank_gaules depois do receive %d\n", rank_gaules);
      sleep(1);
      while(javali>0)
        printf("Sou o Gaules %d, estou comendo um javali e restam %d\n", rank_gaules, --javali);
      //printf("Gaules %d vai pedir pro cozinheiro\n", rank);
      MPI_Send(&fome, sizeof(fome), MPI_CHAR, 0, STD_TAG, MPI_COMM_WORLD);
      //printf("Gaules %d pediu pro cozinheiro\n", rank);
    }
  }
  MPI_Finalize();
}
