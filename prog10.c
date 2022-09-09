#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int var = 10;

void* filho01 () {
   printf("Processo-Filho 01: Iniciando Execucao.\n");
   printf("Processo-Filho 01: O conteudo da variavel VAR e: %d\n", var);
   var = var - 2;
   printf("Processo-Filho 01: O conteudo da variavel VAR e: %d\n", var);
   printf("Processo-Filho 01: Encerrando Execucao.\n");	
}

void* filho02 () {
   printf("Processo-Filho 02: Iniciando Execucao.\n");
   printf("Processo-Filho 02: O conteudo da variavel VAR e: %d\n", var);
   var = var + 3;
   printf("Processo-Filho 02: O conteudo da variavel VAR e: %d\n", var);
   printf("Processo-Filho 02: Encerrando Execucao.\n");	
}

int main() {

  pthread_t	id01, id02;

  system("clear");

  printf("Processo-Pai: Iniciando Execucao.\n");

  printf("Processo-Pai: O conteudo da variavel VAR e: %d\n", var);

  pthread_create(&id01,NULL,&filho01,NULL);
  pthread_create(&id02,NULL,&filho02,NULL);

  printf("Processo-Pai: Iniciando Execucao.\n");

  pthread_join(id01,NULL);
  pthread_join(id02,NULL);

  printf("Processo-Pai: O conteudo da variavel VAR e: %d\n", var);

  printf("Processo-Pai: Encerrando Execucao.\n");  

  return 0;
} 
