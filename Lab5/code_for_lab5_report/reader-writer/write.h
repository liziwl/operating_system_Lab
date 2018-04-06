#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include<semaphore.h>
#include<pthread.h>


sem_t db,rc;
int readcount;

void *writer(int *buffer){
    printf ("write ::%d\n", *buffer);

}

