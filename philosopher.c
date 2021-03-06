#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define nFilosofos 5

sem_t garfo[nFilosofos];
sem_t pedra;


void *filosofo(void *pFilosofo);

int main(){
    pthread_t thread[nFilosofos];
    time_t t;
    srand((unsigned) time(&t));

    sem_init(&pedra, 0, 1);
    for(int nFilosofo = 0; nFilosofo < nFilosofos; nFilosofo++){
        sem_init(&garfo[nFilosofo], 0, 1);

        int *arg = malloc(sizeof(*arg));
        *arg = nFilosofo;
        pthread_create(&thread[nFilosofo], NULL, filosofo, arg);
    }

    for (int i = 0; i < nFilosofos; i++)
        pthread_join(thread[i], NULL);
}

void *filosofo(void *pFilosofo){
    int nFilosofo = *((int *)pFilosofo);
    int maior, menor;

    if(nFilosofo == nFilosofos-1){
        maior = nFilosofos-1;
        menor = 0;
    }else{
        maior = nFilosofo+1;
        menor = nFilosofo;
    }

    printf("Filosofo %d precisa dos garfos %d e %d\n", nFilosofo, maior, menor);
    while(1) {
        if (rand() % 2) {
            printf("Filosofo %d quer comer\n", nFilosofo);

            printf("Filosofo %d aguardando garfo %d\n", nFilosofo, maior);
            sem_wait(&garfo[maior]);

            printf("Filosofo %d aguardando garfo %d\n", nFilosofo, menor);
            sem_wait(&garfo[menor]);

            sem_post(&garfo[maior]);
            sem_post(&garfo[menor]);
            printf("\tFilosofo %d comeu e largou garfo %d e %d\n", nFilosofo, maior, menor);
            sleep(5);
        } else {
            printf("Filosofo %d esta pensando\n", nFilosofo);
            sleep(5);
        }
    }
}

/*
Esta é uma implementação da solução proposta por djikstra porém pegando o garfo com maior número primeiro. Bem parecida com a 
solução usando números pares e ímpares, a diferença é que, nessa solução, seria como se todos pegassem o da direita com a 
exceção do último, que vai pegar o da esquerda. Isso resolve problemas de deadlock, já que mesmo se todos tentarem pegar todos, 
um deles vai ficar sem e outro vai conseguir os dois. Também adicionei a opção de pensar, uma das coisas que o filósofo pode 
fazer
*/
