#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t chopstick[5];
void *f_philosopher(int n)
{
    for (int i = 0; i < 1; i++){
        printf("philosopher %d is thinking !!\n", n + 1);
        pthread_mutex_lock(&chopstick[n]);
        pthread_mutex_lock(&chopstick[(n + 1) % 5]);
        printf("philosopher %d is eating using chopstick[%d] and chopstick[%d] !!\n", n + 1, n, (n + 1) % 5);
        sleep(1);
        pthread_mutex_unlock(&chopstick[n]);
        pthread_mutex_unlock(&chopstick[(n + 1) % 5]);
        printf("philosopher %d finished eating !!\n", n + 1);
    }
}

int main()
{
    pthread_t philosopher[5];
    for (int i = 0; i < 5; i++)
        pthread_mutex_init(&chopstick[i], NULL);

    for (int i = 0; i < 5; i++)
        pthread_create(&philosopher[i], NULL, (void*)f_philosopher, (void*)(intptr_t)i);

    for (int i = 0; i < 5; i++)
        pthread_join(philosopher[i], NULL);

    return 0;
}