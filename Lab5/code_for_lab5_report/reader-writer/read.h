#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include<semaphore.h>
#include<pthread.h>


extern  sem_t db,rc;
extern int readcount;

void *reader(int *buffer){

    printf("\nReader Inside..%d\n", *buffer);
    sleep (3);
    
}

