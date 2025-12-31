#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t customers_waiting; //how many customers are waiting
Zem_t barber_ready; //barbers is ready for a customer
Zem_t done;                
Zem_t mutex;              

int waiting = 0;    //numbers of customers in the waiting area
int num_chairs = 0;  
int num_barbers = 1; //numers of barbers - default barber is 1

void* Barber(void* arg) {
      int id = *(int*)arg;
    while (1) {
        Zem_wait(&customers_waiting); //barber is sleeping
        Zem_wait(&mutex); //barber is waiting for a custimer
        waiting--;
        Zem_post(&barber_ready);  //the barber is ready to do haircut
        Zem_post(&mutex); //barber is cutting hair
        printf("Barber %d is cutting hair\n", id);
        sleep(5);
        printf("Barber %d finished cutting hair\n", id);

        Zem_post(&done); //the customer may leave
    }
    return NULL;
}

void* Customer(void* arg) {
    int id = *(int*)arg;
    Zem_wait(&mutex);

    if (waiting < num_chairs) { //keeping trak of numbers of customers waiting
        waiting++;
        printf("Customer %d sits in the waiting room\n", id);

        Zem_post(&customers_waiting);
        Zem_post(&mutex);

        Zem_wait(&barber_ready);
        printf("Customer %d sits in the barber chair\n", id);
        Zem_wait(&done);
        printf("Customer %d leaves after the haircut\n", id);

    } else {
        printf("Customer %d could not find a chair and left\n", id);
        Zem_post(&mutex);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter the numbers of chairs in the shop\n");
        return 1;
    }
    num_chairs = atoi(argv[1]);
    if (num_chairs <= 0) {
        printf("Number of chairs must be greater than 0\n");
        return 1;
    }

    if(argc >=3){
        num_barbers = atoi(argv[2]);
    }else{
        num_barbers = 1;
    }

    if (num_barbers <= 0) {
        printf("Number of barbers must be greater than 0\n");
        return 1;
    }


    //more customers than the numbers of chairs
    int num_customers = num_chairs + 5;
    printf("Barbershop \n");
    printf("There are %d chairs , %d customers and %d barbers\n", num_chairs, num_customers, num_barbers);

    Zem_init(&customers_waiting, 0); 
    Zem_init(&barber_ready, 0);
    Zem_init(&done, 0);
    Zem_init(&mutex, 1);

    pthread_t barber_threads[100];
    int barbers_ids[100];
    for (int i = 0; i < num_barbers; i++) {
        barbers_ids[i] = i;
        Pthread_create(&barber_threads[i], NULL, Barber, &barbers_ids[i]);
        sleep(1);
    }

    pthread_t customer_threads[100];
    int ids[100];
    for (int i = 0; i < num_customers; i++) {
        ids[i] = i;
        Pthread_create(&customer_threads[i], NULL, Customer, &ids[i]);
        sleep(1);
    }
    for (int i = 0; i < num_customers; i++) {
        Pthread_join(customer_threads[i], NULL);
    }
    return 0;
}
