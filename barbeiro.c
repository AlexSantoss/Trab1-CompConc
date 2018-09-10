#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_CUSTOMERS 10

void *customer(void *num);
void *barber(void *);

sem_t waitingRoom;
sem_t barberChair;
sem_t barberPillow;
sem_t seatBelt;

int allDone = 0;
int waiting = 0;
int numChairs = 5;

int main(int argc, char *argv[]) {
    pthread_t btid;
    pthread_t tid[MAX_CUSTOMERS];
    int i, numCustomers;
    int Number[MAX_CUSTOMERS];

    numCustomers = 7;

    printf("O barbeiro abriu a loja\n");

    for (i=0; i<MAX_CUSTOMERS; i++) {
        Number[i] = i;
    }

    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    pthread_create(&btid, NULL, barber, NULL);

    for (i=0; i<numCustomers; i++) {
        pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
    }

    for (i=0; i<numCustomers; i++) {
        pthread_join(tid[i],NULL);
    }

    allDone = 1;
    sem_post(&barberPillow);
    pthread_join(btid,NULL);
}

void *customer(void *number) {
    int num = *(int *)number;

    printf("Cliente %d chegou na barbearia\n", num);

    waiting++;
    if(waiting > numChairs){
        printf("Sala cheia, %d foi embora\n", num);
        return 0;
    }
    else{
        sem_wait(&waitingRoom);
        printf("Cliente %d entrou na sala de espera\n", num);
    }

    sem_wait(&barberChair);
    sem_post(&waitingRoom);
    waiting--;

    printf("Cliente %d acordou o barbeiro\n", num);
    sem_post(&barberPillow);

    sem_wait(&seatBelt);
    sem_post(&barberChair);

    printf("Cliente %d deixou a barbearia\n", num);
}

void *barber(void *nada) {

    while (!allDone) {

        printf("O barbeiro está dormindo\n");
        sem_wait(&barberPillow);

        if (!allDone) {

            printf("O barbeiro está cortando o cabelo\n");
            sleep(2);
            printf("O barbeiro acabou de cortar o cabelo\n");

            sem_post(&seatBelt);
        }
        else {
            printf("O barbeiro fechou a loja e foi embora\n");
        }
    }
}