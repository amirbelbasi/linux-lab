#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMBER_OF_RESOURCES 5
#define NUMBER_OF_CUSTOMERS 5
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {0};
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
sem_t mutex;
bool is_in_safe_state();
bool need_work(int need_i[], int work[]);

int request_resources(int customer_num, int request[]){
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        if(request[i] > need[customer_num][i] || request[i] > available[i]) return -1;

    sem_wait(&mutex);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }
    if(is_in_safe_state()){
        sem_post(&mutex);
        return 0;
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }
    sem_post(&mutex);
    return -1;
}

int release_resources(int customer_num, int request[]){
    sem_wait(&mutex);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }
    sem_post(&mutex);
}

char *req_to_string(int req[]){
    char *ret = malloc(100);
    char buf[5] = {0};
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
        sprintf(buf, "%d", req[i]);
        strcat(ret, buf);
        strcat(ret, " ");
    }
    return ret;
}

bool is_in_safe_state(){
    int work[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];

    bool finish[NUMBER_OF_CUSTOMERS] = {0};
    int count_finish = 0;
    bool changed;
    while(count_finish != NUMBER_OF_CUSTOMERS){
        changed = false;
        for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
            if (!finish[i] && need_work(need[i], work)){
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    work[j] += allocation[i][j];
                
                finish[i] = true;
                count_finish++;
                changed = true;
            }
        }
        if (!changed) return false;
    }
    return true;
}

bool need_work(int need_i[], int work[]){
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if(need_i[i] > work[i])return false;
    }
    return true;
}

void *customer(int n){
    int req[NUMBER_OF_RESOURCES];
    bool req_rel;
    int done = 0;
    for(int i = 0; i < 10; i++){
        req_rel = rand() % 2;
        if(req_rel == 1){
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                req[j] = rand() % (need[n][j] + 1);
            done = request_resources(n, req);
            printf("Customer #%d requesting   {%s\b} ==> %s\n", n + 1, req_to_string(req), done == 0 ? "request accepted" : "request not accepted");
        }
        else{
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                req[j] = rand() % (allocation[n][j] + 1);
            done = release_resources(n, req);
            printf("Customer #%d releasing    {%s\b}\n", n + 1, req_to_string(req));
        }
    }
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    if(argc < NUMBER_OF_RESOURCES + 1){
        printf("Not enough arguments\n");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = atoi(argv[i + 1]);
    FILE *f_ptr = fopen("matrix.txt","r"); 
    if(f_ptr == NULL){ 
        printf("%s not found.\n", "matrix.txt"); 
        return 0; 
    }
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++){
            fscanf(f_ptr, "%d", &maximum[i][j]);
            need[i][j] = maximum[i][j];
        }
    }
    sem_init(&mutex, 0, 1);
    pthread_t customer_threads[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        pthread_create(&customer_threads[i], NULL, (void *) customer, (void *)(intptr_t) i);

    for (int i = 0; i < 5; i++)
        pthread_join(customer_threads[i], NULL);

    return 0;
}