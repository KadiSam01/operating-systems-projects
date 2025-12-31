#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"
Zem_t forks[20];
int num_philosophers;
int left(int p) {
    return p;
}
int right(int p) {
    return (p + 1) % num_philosophers;
}
void GetForks(int p) {
    int Left = left(p);
    int Right = right(p);
    Zem_wait(&forks[Left]);
    printf("Philosopher %d picks up the left fork\n", p);
    sleep(1);
    Zem_wait(&forks[Right]);
    printf("Philosopher %d picks up the right fork\n", p);
}

void PutForks(int p) {
    int Left = left(p);
    int Right = right(p);
    Zem_post(&forks[Left]);
    Zem_post(&forks[Right]);
    printf("Philosopher %d puts down both forks\n", p);
}
void* DiningPhilosopher(void* arg) {
    int p = *(int*)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", p);
        sleep(1);
        printf("Philosopher %d wants to eat\n", p);
        GetForks(p);
        printf("Philosopher %d is eating\n", p);
        sleep(1);
        PutForks(p);
    }
    return NULL;
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter the number of philosophers.\n");
        return 1;
    }
    num_philosophers = atoi(argv[1]);
    if (num_philosophers < 3 || num_philosophers > 20) {
        printf("pick a number between 3 and 20.\n");
        return 1;
    }

    for (int i = 0; i < num_philosophers; i++) {
        Zem_init(&forks[i], 1);
    }
    pthread_t threads[20];
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
