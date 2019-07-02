#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#define N 5

pthread_t tid[N];
sem_t S1, S2, S3;

void *A(void *arg){
  //sem_wait(&S1);
  printf("Iniciando A\n");
  //sleep(rand()%5);
  printf("Finalizando A\n");
  sem_post(&S1);
}

void *B(void *arg){
  //sem_wait(&S2);
  printf("Iniciando B\n");
  //sleep(rand()%5);
  printf("Finalizando B\n");
  sem_post(&S2);
}

void *C(void *arg){
  sem_wait(&S1);
  sem_wait(&S2);
  //sem_wait(&S3);
  printf("Iniciando C\n");
  //sleep(rand()%5);
  printf("Finalizando C\n");
  //sem_post(&S1);
  //sem_post(&S2);
  sem_post(&S3);
  sem_post(&S3);
}

void *D(void *arg){
  sem_wait(&S3);
  printf("Iniciando D\n");
  //sleep(rand()%5);
  printf("Finalizando D\n");
}

void *E(void *arg){
  sem_wait(&S3);
  printf("Iniciando E\n");
  //sleep(rand()%5);
  printf("Finalizando E\n");
}

int main(){
  sem_init(&S1, 0, 0);
  sem_init(&S2, 0, 0);
  sem_init(&S3, 0, 0);
  pthread_create(&tid[0], NULL, A, NULL);
  pthread_create(&tid[1], NULL, B, NULL);
  pthread_create(&tid[2], NULL, C, NULL);
  pthread_create(&tid[3], NULL, D, NULL);
  pthread_create(&tid[4], NULL, E, NULL);
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  pthread_join(tid[3], NULL);
  pthread_join(tid[4], NULL);

  return 0;
}
