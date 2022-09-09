#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* filho01 () {
   printf("Processo-Filho 01: Iniciando Execucao.\n");
   sleep(5);
   printf("Processo-Filho 01: Encerrando Execucao.\n");	
}

void* filho02 () {
   printf("Processo-Filho 02: Iniciando Execucao.\n");
   sleep(5);
   printf("Processo-Filho 02: Encerrando Execucao.\n");	
}

int main() {

  pthread_t	id01, id02;

  system("clear");

  printf("Processo-Pai: Iniciando Execucao.\n");

  pthread_create(&id01,NULL,&filho01,NULL);
  pthread_create(&id02,NULL,&filho02,NULL);

  pthread_join(id01,NULL);
  pthread_join(id02,NULL);

  printf("Processo-Pai: Encerrando Execucao.\n");  

  return 0;
} 
