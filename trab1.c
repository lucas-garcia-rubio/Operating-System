#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
	pid_t pid;
	int i;

	printf("Olá, sou o processo A, e meu PID é %d!\n", getpid());
	pid = fork();
	if(pid == 0){
		printf("Olá, sou o processo B! PID = %d  PPID = %d\n", getpid(), getppid());
		pid = fork();
		if(pid == 0){
			printf("Olá, sou o processo C! PID = %d  PPID = %d\n", getpid(), getppid());
			printf("Olá, sou o processo C e vou morrer! PID = %d  PPID = %d\n", getpid(), getppid());
		}
		else{
			wait(NULL);
			printf("Sou o processo B! Meu filho C morreu! Vou me matar!  PID = %d  PPID = %d\n", getpid(), getppid());
		}
	}
	else{
		wait(NULL);
		pid = fork();
		if(pid == 0){
			printf("Olá, sou o processo D! PID = %d  PPID = %d\n", getpid(), getppid());
			printf("Meu Deus, sou o D e já vou morrer! PID = %d  PPID = %d\n", getpid(), getppid());
		}
		else{
			wait(NULL);
			pid = fork();
			if(pid == 0){
				printf("Olá, sou o processo E! PID = %d  PPID = %d\n", getpid(), getppid());
				pid = fork();
				if(pid == 0){
					printf("Olá, sou o processo F! PID = %d  PPID = %d\n", getpid(), getppid());
					pid = fork();
					if(pid == 0){
						printf("Olá, sou o processo G! PID = %d  PPID = %d\n", getpid(), getppid());
						printf("Oh não! Mal nasci e já vou morrer! G deixa esse SO! PID = %d  PPID = %d\n", getpid(), getppid());
					}
					else{
						wait(NULL);
						printf("Sou o processo F e vou morrer agora! PID = %d  PPID = %d\n", getpid(), getppid());
					}
				}
				else{
					wait(NULL);
					printf("Olá! Sou o E e estava esperando meu filho morrer! Agora já posso ir! PID = %d  PPID = %d\n", getpid(), getppid());
				}
			}
			else{
				wait(NULL);
				printf("Sou o A e já estou bem velhinho! Todos meus filhos morreram! Adeus! PID = %d  PPID = %d\n", getpid(), getppid());
			}
		}
	}
}
