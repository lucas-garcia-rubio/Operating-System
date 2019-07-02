# include <sys/types.h>
# include <signal.h>
# include <stdio.h>

int n = 0;

void tratamento(int sigNumb){
  if(sigNumb == SIGINT){
    n++;
    if(n == 10){
      printf("Really exit? (Y/n)\n");
      alarm(5);
      char a;
      scanf(" %c", &a);
      if(a == 'Y')
        raise(SIGABRT);
      else{
        n = 0;
        alarm(0);
      }
    }
  }
  else if(sigNumb == SIGTSTP){
    printf("n = %d\n", n);
  }
}

int main(){
  signal(SIGINT, tratamento);
  signal(SIGTSTP, tratamento);
  for(;;)
  {

  }
}
