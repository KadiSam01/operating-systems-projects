
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t forks[50];
Zem_t table;           
int num_philosophers;

int left(int p) {
    return p;
}
int right(int p) {
    return (p + 1) % num_philosophers;
}

void GetForks(int p) {
    int Right = right(p);
    int Left = left(p);

    Zem_wait(&forks[Right]);
    Zem_wait(&forks[Left]);
}

void PutForks(int p) {
    int Right = right(p);
    int Left = left(p);
    Zem_post(&forks[Right]);
    Zem_post(&forks[Left]);
}

void GetTable(void) { //only N-1 may sit on the table
    Zem_wait(&table);
}
void LeaveTable(void) { //frees a table for another philospher to sit after taking both of their forks 
    Zem_post(&table);
}

void* DiningPhilosopher(void* arg) {
    int p = *(int*)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", p);
        sleep(1);
        GetTable();
        printf("Philosopher %d sits at the table\n", p);
        printf("Philosopher %d is picking up forks\n", p);
        GetForks(p);
        printf("Philosopher %d is eating\n", p);
        sleep(1);
        PutForks(p);
        printf("Philosopher %d put down forks\n", p);
        LeaveTable();
        printf("Philosopher %d leaves the table\n", p);
    }
    return NULL;
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter the number of philosophers.\n");
        return 1;
    }
    num_philosophers = atoi(argv[1]);
    if (num_philosophers < 3 || num_philosophers > 50) {
        printf("pick  a number between 3 and 50.\n");
        return 1;
    }
    for (int i = 0; i < num_philosophers; i++) {
        Zem_init(&forks[i], 1);
    }
    Zem_init(&table, num_philosophers - 1); //N-1
    pthread_t threads[50];
    int ids[50];
    for (int i = 0; i < num_philosophers; i++) {
        ids[i] = i;
        Pthread_create(&threads[i], NULL, DiningPhilosopher, &ids[i]);
    }
    for (int i = 0; i < num_philosophers; i++) {
        Pthread_join(threads[i], NULL);
    }
    return 0;
}
