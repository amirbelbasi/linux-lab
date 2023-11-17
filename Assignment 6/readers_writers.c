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

int id;
int base_pid;

typedef struct{
    int count, read_count;
    pthread_mutex_t mutex;
    pthread_mutex_t rw_mutex;
} Shared;

void reader(){
    Shared* shared;
    shared = (Shared *)shmat(id, NULL, 0);
    if (shared == (Shared*)-1){
        perror("shmat perror");
        exit(1);
    }
    int pid = getpid();
    bool isDone = false;
    while (!isDone){
        pthread_mutex_lock(&(shared->mutex));
        (shared->read_count)++;
        if (shared->read_count == 1) pthread_mutex_lock(&(shared->rw_mutex));
        pthread_mutex_unlock(&(shared->mutex));
        printf("For Reader:\nPID: %d    Count: %d\n\n", pid, shared->count);
        if (shared->count >= 2) isDone = true;
        pthread_mutex_lock(&(shared->mutex));
        (shared->read_count)--;
        if (shared->read_count == 0) pthread_mutex_unlock(&(shared->rw_mutex));
        pthread_mutex_unlock(&(shared->mutex));
        sleep(0.1);
    }
    if(shmdt(shared) == -1){
        perror("shmdt perror");
        exit(1);
    }
}

void writer() {
    Shared* shared;
    shared = (Shared *)shmat(id, NULL, 0);
    if(shared == (Shared *) -1){
        perror("shmat perror");
        exit(1);
    }
    int pid = getpid();
    bool isDone = false;
    while(!isDone){
        pthread_mutex_lock(&(shared->rw_mutex));
        shared->count++;
        printf("For Writer:\nPID: %d    Count: %d\n\n", pid, shared->count);
        if (shared->count >= 2) isDone = true;
        pthread_mutex_unlock(&(shared->rw_mutex));
        sleep(0.05);
    }
    if(shmdt(shared) == -1){
        perror("shmdt perror");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    int pid;
    id = shmget(IPC_PRIVATE, sizeof(Shared), IPC_CREAT | 0666);
    if(id < 0){
        perror("shmget perror");
        exit(-1);
    }
    Shared* at;
    at = (Shared*)shmat(id, NULL, 0);
    if(at == (Shared*)-1){
        perror("shmat perror");
        exit(1);
    }
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(at->mutex), &attr);
    pthread_mutex_init(&(at->rw_mutex), &attr);
    if (shmdt(at) == -1){
        perror("shmdt perror");
        exit(1);
    }
    base_pid = getpid();
    pid = fork();
    if (pid == 0){
        writer();
        return 0;
    }
    for(int i = 0; i < 2; i++){
        if(getpid() == base_pid) pid = fork();
        else break;
    }
    if(pid == 0){
        reader();
        return 0;
    }
    if(getpid() == base_pid){
        wait(NULL);
        for(int i = 0; i < 2; i++){
            wait(NULL);
        }
    }
    if(shmctl(id, IPC_RMID, NULL) == -1){   
        perror("shmctl perror");
        exit(1);
    }   
    return 0;
}