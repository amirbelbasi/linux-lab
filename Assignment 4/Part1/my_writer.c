#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 100

int main()
{
    char *ptr;
    key_t key;
    int id;
    key = 1234;
    char *messageToSend = "Hello World!";
    id = shmget(key, SIZE, IPC_CREAT | 0666);
    if(id < 0){
        perror("shmget perror");
        exit(1);
    }
    ptr = shmat(id, NULL, 0);
    if(ptr == (char*)-1){
        perror("shmat perror");
        exit(1);
    }
    sprintf(ptr, "%s", messageToSend);
    printf("Waiting for reader:\n");
    while (*ptr != '!') sleep(0.5);
    printf("Message sent\nClearing memory\n");
    if(shmdt(ptr) == -1){
        perror("shmdt perror");
        exit(1);
    }
    if(shmctl(id, IPC_RMID, NULL) == -1){   
        perror("shmctl perror");
        exit(1);
    }
}