#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include<semaphore.h>
#include<pthread.h>


sem_t *db,*rc;
int readcount;

void *writer(void *buffer)
{
	sem_wait(db);
	printf ("write ::%d\n", ++(*((int *)buffer)));
	sem_post(db);
}

