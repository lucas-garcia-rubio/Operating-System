// Jantar dos Gauleses - Pipe

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

# define M 18
# define N 5
# define WRITE 1
# define READ 0

pthread_mutex_t mutex;

int qntd_javali = 0;

int p1[2], p2[2], p3[2];

void RetiraJavali(void *id_gaules){
  long id = (long)id_gaules;
  if(id == 0)
    printf("Gaules L(%ld) comendo javali, restam %d javalis\n", id, --qntd_javali);
  else if(id == 1)
    printf("Gaules u(%ld) comendo javali, restam %d javalis\n", id, --qntd_javali);
  else if(id == 2)
    printf("Gaules c(%ld) comendo javali, restam %d javalis\n", id, --qntd_javali);
  else if(id == 3)
    printf("Gaules a(%ld) comendo javali, restam %d javalis\n", id, --qntd_javali);
  else
    printf("Gaules s(%ld) comendo javali, restam %d javalis\n", id, --qntd_javali);
}

void *Gauleses(void *args){
  while(1){
    pthread_mutex_lock(&mutex);
    if(qntd_javali == 0){
      int x = 1; //um valor qualquer, somente para acordar o cozinheiro
      write(p2[WRITE], &x, sizeof(int));
      read(p1[READ], &qntd_javali, sizeof(int));
    }
    RetiraJavali(args);
    pthread_mutex_unlock(&mutex);
    sleep(1);
  }
}

void ColocaJavali(int x){
  printf("Cozinheiro colocou mais %d javalis\n", x);
  write(p1[WRITE], &x, sizeof(int));
}

void *Cozinheiro(void *args){
  while(1){
    int x;
    read(p2[READ], &x, sizeof(int));
    ColocaJavali(M);
  }
}

int main(){
  pid_t pid;
  long i;

  pipe(p1); //pipe da mesa, onde o cozinheiro vai colocar os javalis
  pipe(p2); //pipe para acordar o cozinheiro. Cozinheiro vai tentar ler, mas como estará vazio, adormecerá até que
            //algum gaulês coloque alguma coisa lá
  pipe(p3); //cozinheiro coloca alguma coisa nesse pipe quando terminar de encher o pipe 2 para acordar os gauleses

  printf("mandou o alou 1\n");
  pid=fork();
  if(pid < 0){ //não conseguiu criar o processo filho
    printf("não conseguiu criar o fork\n");
    exit(1);
  }
  else if(pid==0){ //processo filho -> Gaules
    printf("mandou o alou 2 do pid: %d\n", getpid());
    close(p2[READ]);
    close(p1[WRITE]);
    pthread_mutex_init(&mutex, NULL);

    pthread_t gauleses[N];
    for(i=0; i<N; i++)
      pthread_create(&gauleses[i], NULL, Gauleses, (void*)i);
    printf("mandou os gauleses\n");
    for(i=0; i<N; i++)
      pthread_join(gauleses[i], NULL);
  }
  else{ //processo pai -> Cozinheiro
    close(p2[WRITE]);
    close(p1[READ]);

    pthread_t cozinheiro;

    pthread_create(&cozinheiro, NULL, Cozinheiro, NULL);
    printf("mandou o cozinheiro\n");
    pthread_join(cozinheiro, NULL);
  }
}
