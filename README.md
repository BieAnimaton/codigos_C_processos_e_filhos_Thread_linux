
# Criando processos e filhos 
Códigos feitos em C para criar e manipular processos Threads e filhos Threas no Linux (distro usada: Slax)

# Observações adicionais:

|     Função    | Processo Tradicional | Processo Thread |
| :------------ |    :-----------:     |  ------------:  |
| Incluindo Biblioteca | `#inlude <sys/types.h>` | `#include <pthread.h>` |
| Criando filho(s) | `fork();` | `pthread_create()` |
| Aguardando filho(s) ficar livre | `waitpid()` | `pthread_join()` |

Para compilar: `cc progN.c -o progN **-lpthread**`  
Para executar: `./progN`
## Comando para criar processo filho através do Gerenciador de Processos.
```
...
pthread_create(&thread_id, NULL, &filho, NULL);
...
```
Para usar corretamente, devemos conhecer seus parâmetros:  

`pthread_create(&thread_id , NULL , &filho , NULL);`  
`pthread_create(PID da Thread , qual é o algoritmo de escalonamento , função executada pela Thread , mensagem/variável enviada para a Thread);`  

- O 1º valor de NULL significa que a Thread vai usar o **mesmo algoritmo de escalonamento que o Sistema Operacional**.  
- O 2º valor de NULL significa que **nenhuma mensagem/variável será enviada**, ou seja, nulo.
## Criando a variável "thread_id" do tipo "pthread_t" para armazenar o PID da Thread retornada pelo "pthread_create()".
```
...
pthread_t thread_id;
...
pthread_create(&thread_id, NULL, &filho, NULL);
...
```

## Criar função para que o filho execute.
```
...
void* filho () {
   printf("Processo-Filho: Iniciando Execução.\n");
   sleep(5);
   printf("Processo-Filho: Encerrando Execução.\n");
}
...
pthread_t thread_id;
...
printf("Processo-Pai: Iniciando Execução.\n");
pthread_create(&thread_id, NULL, &filho, NULL);
printf("Processo-Pai: Encerrando Execução.\n");
...
```
Ao executarmos este código, nos deparamos com um **erro**: o pai inicializa Imprimindo "Processo-Pai: Iniciando Execução", em seguida o filho inicializa Imprimindo "Processo-Filho: Iniciando Execução." e o pai terminar Imprimindo "Processo-Pai: Encerrando Execução".  
＼（〇_ｏ）／  

Você percebeu que o filho não imprimiu sua segunda mensagem?! Isso acontece por que nos processos Threads, **quando o pai "morre", todos os seus filhos também "morrem"**, ou seja, o pai precisa ter um tempo de espera maior que o do filho para que este último possa imprimir todo seu conteúdo.
## Criar função para que o filho execute, fazendo o pai aguardar o filho I.
```
...
void* filho () {
   printf("Processo-Filho: Iniciando Execução.\n");
   sleep(5);
   printf("Processo-Filho: Encerrando Execução.\n");
}
...
pthread_t thread_id;
...
printf("Processo-Pai: Iniciando Execução.\n");
pthread_create(&thread_id, NULL, &filho, NULL);
sleep(10);
printf("Processo-Pai: Encerrando Execução.\n");
...
```
O acréscimo do `sleep(10);` faz o pai aguardar o filho completar seu trabalho, não o matando instantaneamente.
## Usar "sleep(N);" não é recomendado - usar "pthread_join(x, y);.
```
...
void* filho () {
   printf("Processo-Filho: Iniciando Execução.\n");
   sleep(5);
   printf("Processo-Filho: Encerrando Execução.\n");
}
...
pthread_t thread_id;
...
printf("Processo-Pai: Iniciando Execução.\n");
pthread_create(&thread_id, NULL, &filho, NULL);
pthread_join(thread_id, NULL);
printf("Processo-Pai: Encerrando Execução.\n");
...
```
`pthread_join(thread_id , NULL);`  
`pthread_join(PID da Thread , estado do filho);`  
O comando `thread_id()` segue a seguinte lógica: **quando o estado de "thread_id" for NULL, encerrar o filho**.  
## Variáveis **SÃO** compartilhadas I.
Agora estamos trabalhando com processos **THREADS**, ou seja, o(s) filho(s) **compartilham variáveis com o pai**.
```
#include <string.h>
...
void* filho (void* mensagem) {
   printf("Processo-Filho: Iniciando Execução.\n");
   printf("Processo-Filho: O conteúdo da variável MENSAGEM é foi %s\n", mensagem);
   strcpy(mensagem, "MSG do Proc. Filho");
   sleep(5);
   printf("Processo-Filho: O conteúdo da variável MENSAGEM é foi %s\n", mensagem);
   printf("Processo-Filho: Encerrando Execução.\n");
}
...
pthread_t thread_id;
char mensagem[80] = "MSG do Proc. Pai";  // local
...
printf("Processo-Pai: Iniciando Execução.\n");
printf("O conteúdo da variável MENSAGEM é foi %s\n", mensagem);

pthread_create(&thread_id, NULL, &filho, NULL);
pthread_join(thread_id, NULL);

printf("O conteúdo da variável MENSAGEM é foi %s\n", mensagem);
printf("Processo-Pai: Encerrando Execução.\n");
...
```
No exemplo acima, como os dados são compartilhados, ou seja, a mesma variável é usada pelos dois, a saída na tela ficará da seguinte forma:  
- **"pai" vai imprimir "MSG do Proc. Pai"**,
- **"filho" vai imprimir "MSG do Proc. Pai"**,
- Depois da alteração pelo `strcpy();`,
- **"filho" vai imprimir "MSG do Proc. Filho"**,
- E como a variável é a mesma,
- **"pai" vai imprimir "MSG do Proc. Filho"**
## Variáveis **SÃO** compartilhadas II.
```
#include <string.h>
...
char mensagem[80] = "MSG do Proc. Pai"; // global
...
void* filho (void* mensagem) {
   printf("Processo-Filho: Iniciando Execução.\n");
   printf("Processo-Filho: O conteúdo da variável MENSAGEM é foi %s\n", mensagem);
   strcpy(mensagem, "MSG do Proc. Filho");
   sleep(5);
   printf("Processo-Filho: O conteúdo da variável MENSAGEM é foi %s\n", mensagem);
   printf("Processo-Filho: Encerrando Execução.\n");
}
...
pthread_t thread_id;
...
printf("Processo-Pai: Iniciando Execução.\n");
printf("O conteúdo da variável MENSAGEM é foi %s\n", mensagem);

pthread_create(&thread_id, NULL, &filho, NULL);
pthread_join(thread_id, NULL);

printf("O conteúdo da variável MENSAGEM é foi %s\n", mensagem);
printf("Processo-Pai: Encerrando Execução.\n");
...
```
No exemplo acima, como os dados são compartilhados, ou seja, a mesma variável é usada pelos dois **E É GLOBAL**, a saída na tela ficará da mesma forma:  
- **"pai" vai imprimir "MSG do Proc. Pai"**,
- **"filho" vai imprimir "MSG do Proc. Pai"**,
- Depois da alteração pelo `strcpy();`,
- **"filho" vai imprimir "MSG do Proc. Filho"**,
- E como a variável é a mesma,
- **"pai" vai imprimir "MSG do Proc. Filho"**
## Variáveis **SÃO** compartilhadas III.
```
int var = 10 // global
...
void* filho01 (void* mensagem) {
   printf("Processo-Filho 01: Iniciando Execução.\n");
   printf("Processo-Filho 01: O conteúdo da variável VAR é %d\n", var);
   var = var - 2;
   sleep(5);
   printf("Processo-Filho 01: O conteúdo da variável VAR é %d\n", var);
   printf("Processo-Filho 01: Encerrando Execução.\n");
}

void* filho02 (void* mensagem) {
   printf("Processo-Filho 02: Iniciando Execução.\n");
   printf("Processo-Filho 02: O conteúdo da variável VAR é %d\n", var);
   var = var + 3;
   sleep(5);
   printf("Processo-Filho 02: O conteúdo da variável VAR é %d\n", var);
   printf("Processo-Filho 02: Encerrando Execução.\n");
}
...
pthread_t thread_id;
...
printf("Processo-Pai: Iniciando Execução.\n");
printf("O conteúdo da variável VAR é %d\n", var);

pthread_create(&thread_id, NULL, &filho, NULL);
pthread_join(thread_id, NULL);

printf("O conteúdo da variável VAR é %d\n", var);
printf("Processo-Pai: Encerrando Execução.\n");
...
```
No exemplo acima, como os dados são compartilhados, ou seja, a mesma variável é usada pelos dois **E É GLOBAL**, a saída na tela ficará da mesma forma:  
- **"pai" vai imprimir 10**,
- **"filho 01" vai imprimir 10**,
- **"filho 01" vai imprimir 8**,
- **"filho 02" vai imprimir 8**,
- **"filho 02" vai imprimir 11**,
- **"pai" vai imprimir 11**