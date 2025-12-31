#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t forks[20];          
int fork_free[20];        
int first_fork[20];       
int second_fork[20];     
Zem_t fork_lock;        
int num_philosophers;

int left(int p) {
    return p;
}
int right(int p) {
    return (p + 1) % num_philosophers;
}

void GetForksNormal(int p) {
    int Left = left(p);
    int Right = right(p);

    Zem_wait(&forks[Left]);
    Zem_wait(&fork_lock);
    fork_free[Left] = 0;
    Zem_post(&fork_lock);
    first_fork[p] = Left;

    sleep(1);

    Zem_wait(&forks[Right]);
    Zem_wait(&fork_lock);
    fork_free[Right] = 0;
    Zem_post(&fork_lock);
    second_fork[p] = Right;
    printf("Philosopher %d (normal) picks up forks %d and %d\n", p, Left, Right);
}

void GetForksRogue(int p) {
    int Left = left(p);
    Zem_wait(&forks[Left]);
    Zem_wait(&fork_lock);
    fork_free[Left] = 0;
    Zem_post(&fork_lock);
    first_fork[p] = Left;

    printf("Philosopher %d (rogue) picked up first fork %d\n", p, Left);

    int second = -1;
    while (1) {
        Zem_wait(&fork_lock);
        for (int i = 0; i < num_philosophers; i++) { //the rogue philsophers is checking for any free forks - not just whats on the right side
            if (i == Left) {
                continue; 
            }
            if (fork_free[i] == 1) {
                fork_free[i] = 0;  
                second = i;
                break;
            }
        }
        Zem_post(&fork_lock);
        if (second != -1) {
            Zem_wait(&forks[second]);
            second_fork[p] = second;
            printf("Philosopher %d (rogue) picks up the second fork %d\n", p, second);
            break;
        }
        sleep(1);
    }
}
void PutForks(int p) {
    int f1 = first_fork[p];
    int f2 = second_fork[p];
    Zem_wait(&fork_lock);
    fork_free[f1] = 1;
    fork_free[f2] = 1;
    Zem_post(&fork_lock);
    Zem_post(&forks[f1]);
    Zem_post(&forks[f2]);
    printf("Philosopher %d put down forks %d and %d\n", p, f1, f2);
}
void* DiningPhilosopher(void* arg) {
    int p = *(int*)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", p);
        sleep(1);
        printf("Philosopher %d wants to eat\n", p);
     
        if (p == 0) {
            GetForksRogue(p); //only philosopher 0 acts rogue
        } else {
            GetForksNormal(p);
        }
        printf("Philosopher %d is eating\n", p); //the others act normal
        sleep(1);
        PutForks(p);
    }
    return NULL;
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please enter the number of philosophers.\n");
        return 1;
    }
    num_philosophers = atoi(argv[1]);
    if (num_philosophers < 3 || num_philosophers > 20) {
        printf("Please choose a number between 3 and 20.\n");
        return 1;
    }
    for (int i = 0; i < num_philosophers; i++) {
        Zem_init(&forks[i], 1);
        fork_free[i] = 1;  
        first_fork[i] = -1;
        second_fork[i] = -1;
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
