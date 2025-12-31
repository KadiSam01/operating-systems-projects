#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t forks[20];      
int fork_free[20];    
Zem_t fork_lock;       
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
    while (1) {
        Zem_wait(&fork_lock);
        if (fork_free[Left] == 1 && fork_free[Right] == 1) {
            fork_free[Left] = 0;
            fork_free[Right] = 0;
            Zem_post(&fork_lock);
            break; 
        }
        Zem_post(&fork_lock);
        sleep(1);
    }
    Zem_wait(&forks[Left]);
    Zem_wait(&forks[Right]);
    printf("Philosopher %d picked up both forks\n", p);
}
void PutForks(int p) {
    int Left = left(p);
    int Right = right(p);
    Zem_wait(&fork_lock);
    fork_free[Left] = 1; 
    fork_free[Right] = 1;
    Zem_post(&fork_lock);
    Zem_post(&forks[Left]);
    Zem_post(&forks[Right]);

    printf("Philosopher %d put down both forks\n", p);
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
        printf("Please enter the number of philosophers\n");
        return 1;
    }
    num_philosophers = atoi(argv[1]);
    if (num_philosophers < 3 || num_philosophers > 20) {
        printf("Please choose a number between 3 and 20\n");
        return 1;
    }
    for (int i = 0; i < num_philosophers; i++) {
        Zem_init(&forks[i], 1);
        fork_free[i] = 1;  
    }
    Zem_init(&fork_lock, 1);
    pthread_t threads[20];
    int ids[20];
    for (int i = 0; i < num_philosophers; i++) {
        ids[i] = i;
        Pthread_create(&threads[i], NULL, DiningPhilosopher, &ids[i]);
    }
    for (int i = 0; i < num_philosophers; i++) {
        Pthread_join(threads[i], NULL);
    }
    return 0;
}
