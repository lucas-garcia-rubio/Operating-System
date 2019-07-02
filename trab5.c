#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
# define M 18
# define N 5

pthread_t gauleses[N];
pthread_t cozinheiro;

sem_t cozinha, comida, enchendo;
pthread_mutex_t mutex;

int qntd_javali=M;

void ColocaJavali(int x){
  qntd_javali = x;
  printf("Cozinheiro colocou mais %d javalis\n", x);
  int i;
  for(i=0; i<M; i++)
    sem_post(&comida);
}

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

void inicializa_semaforos(){
  pthread_mutex_init(&mutex, NULL);
  sem_init(&cozinha, 0, 0);
  sem_init(&comida, 0, M);
  sem_init(&enchendo, 0, 0);
}

void *Cozinheiro(void *args){
  while(1){
    sem_wait(&cozinha);
    ColocaJavali(M);
    sem_post(&enchendo);
  }
}

void *Gauleses(void *args){
  while(1){
    sem_wait(&comida);
    pthread_mutex_lock(&mutex);
    RetiraJavali(args);
    if(qntd_javali == 0){
      sem_post(&cozinha);
      sem_wait(&enchendo);
    }
    pthread_mutex_unlock(&mutex);
  }
}

int main(){
  inicializa_semaforos();
  long i;

  pthread_create(&cozinheiro, NULL, Cozinheiro, NULL);

  for(i=0; i<N; i++)
    pthread_create(&gauleses[i], NULL, Gauleses, (void*)i);

  pthread_join(cozinheiro, NULL);
  for(i=0; i<N; i++)
    pthread_join(gauleses[i], NULL);
}
