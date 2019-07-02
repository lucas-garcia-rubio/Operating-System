//Shared Memory - Cozinheiro

# include <sys/ipc.h>
# include <sys/shm.h>
# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
# include <unistd.h>
# define M 18

pthread_t cozinheiro;

int *qntd_javali; //terá uma região de memória compartilhada para a quantidade de javalis
sem_t *cozinha, *comida, *enchendo; //uma região de memória para cada semáforo da mesa
pthread_mutex_t *mutex;

void inicializa_semaforos(){
  sem_init(cozinha, 1, 0);
  sem_init(comida, 1, M);
  sem_init(enchendo, 1, 0);
  pthread_mutex_init(mutex, NULL);
  printf("semáforos inicializados C\n");
}

void ColocaJavali(int x){
  printf("Cozinheiro colocou mais %d javalis\n", x);
  int i;
  for(i=0; i<M; i++)
    sem_post(comida);
  *qntd_javali = x;
}

void *Cozinheiro(void *args){
  while(1){
    sem_wait(cozinha);
    ColocaJavali(M);
    sem_post(enchendo);
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
  qntd_javali = (int*) shmat(shmid, 0, 0);
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

  pthread_create(&cozinheiro, NULL, Cozinheiro, NULL);
  pthread_join(cozinheiro, NULL);
}
