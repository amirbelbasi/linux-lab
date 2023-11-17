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
    id = shmget(key, SIZE, 0666);
    if(id < 0){
        perror("shmget perror");
        exit(1);
    }
    ptr = shmat(id, NULL, 0);
    if(ptr == (char*)-1){
        perror("shmat perror");
        exit(1);
    }
    printf("Message recived: %s\n", ptr);
    *ptr = '!';
}