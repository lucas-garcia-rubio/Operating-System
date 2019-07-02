//Shared Memory - Gaulês

# include <sys/ipc.h>
# include <sys/shm.h>
# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
# include <unistd.h>
# define N 5
# define M 18

pthread_t gauleses[N];

int *qntd_javali; //terá uma região de memória compartilhada para a quantidade de javalis
sem_t *cozinha, *comida, *enchendo; //uma região de memória para cada semáforo da mesa
pthread_mutex_t *mutex;

void inicializa_semaforos(){
  pthread_mutex_init(mutex, NULL);
  sem_init(cozinha, 1, 0);
  sem_init(comida, 1, M);
  sem_init(enchendo, 1, 0);
  printf("semáforos inicializados G\n");
}

void RetiraJavali(void *id_gaules){
  long id = (long)id_gaules;
  if(id == 0)
    printf("Gaules L(%ld) comendo javali, restam %d javalis\n", id, --*qntd_javali);
  else if(id == 1)
    printf("Gaules u(%ld) comendo javali, restam %d javalis\n", id, --*qntd_javali);
  else if(id == 2)
    printf("Gaules c(%ld) comendo javali, restam %d javalis\n", id, --*qntd_javali);
  else if(id == 3)
    printf("Gaules a(%ld) comendo javali, restam %d javalis\n", id, --*qntd_javali);
  else
    printf("Gaules s(%ld) comendo javali, restam %d javalis\n", id, --*qntd_javali);
}

void *Gauleses(void *args){
  while(1){
    sem_wait(comida);
    pthread_mutex_lock(mutex);
    RetiraJavali(args);
    if(*qntd_javali == 0){
      sem_post(cozinha);
      sem_wait(enchendo);
    }
    pthread_mutex_unlock(mutex);
  }
}

int main(){
  int shmid;
  key_t key_mesa = 1;
  key_t key_cozinha = 2;
  key_t key_comida = 3;
  key_t key_enchendo = 4;
  key_t key_mutex = 6;

  shmid = shmget(key_mesa, sizeof(int), IPC_CREAT | SHM_R | SHM_W);
  qntd_javali = (int*)shmat(shmid, 0, 0);
  *qntd_javali = M;
  shmid = shmget(key_cozinha, sizeof(sem_t),  IPC_CREAT | SHM_R | SHM_W);
  cozinha = (sem_t*) shmat(shmid, 0, 0);
  shmid = shmget(key_comida, sizeof(sem_t),  IPC_CREAT | SHM_R | SHM_W);
  comida = (sem_t*) shmat(shmid, 0, 0);
  shmid = shmget(key_enchendo, sizeof(sem_t),  IPC_CREAT | SHM_R | SHM_W);
  enchendo = (sem_t*) shmat(shmid, 0, 0);
  shmid = shmget(key_mutex, sizeof(pthread_mutex_t),  IPC_CREAT | SHM_R | SHM_W);
  mutex = (pthread_mutex_t*) shmat(shmid, 0, 0);

  inicializa_semaforos();

  long i;
  for(i=0; i<N; i++)
    pthread_create(&gauleses[i], NULL, Gauleses, (void*)i);
  for(i=0; i<N; i++)
    pthread_join(gauleses[i], NULL);
}
