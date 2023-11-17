#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void printHistogram(int hist[])
{
    for (int i = 0; i < 25; i++)
    {
        for(int j = 0; j < hist[i]; j++) {
          printf("*");
        }
        printf("\n");
    }
}

typedef struct
{
    int data[25];
} Hist;

int main(int argc, char const *argv[])
{
    Hist *hist;
    int id;
    int base_pid;
    int pid;
    int samples;
    if (argc == 2) samples = atoi(argv[1]);
    srand(time(NULL));
    id = shmget(IPC_PRIVATE, sizeof(Hist), IPC_CREAT | 0666);
    if (id < 0)
    {
        printf("error");
        exit(-1);
    }
    base_pid = getpid();
    for (int i = 0; i < 5; i++)
    {
        if (getpid() == base_pid) pid = fork();
        else break;
    }
    hist = (Hist*)shmat(id, NULL, 0);
    if ((hist) == (Hist*)-1)
    {
        printf("error");
        exit(-1);
    }
    if (getpid() == base_pid)
    {
        for (int i = 0; i < 5; i++) wait(NULL);
    }
    else
    {
        int counter, rnd;
        for (int i = 0; i < samples / 5; i++)
        {
            counter = 0;
            for (int j = 0; j < 12; j++)
            {
                rnd = rand() % 101;
                if (rnd >= 49) counter++;
                else counter--;
            }
            hist->data[counter + 12]++;
        }
        exit(0);
    }
    //printHistogram(hist->data);
    return 0;
}