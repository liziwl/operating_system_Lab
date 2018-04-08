#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

extern  sem_t *db,*rc;
extern int readcount;

void *reader(void *buffer)
{
	sem_wait(rc);
	if(readcount == 0)
		sem_wait(db);
	++readcount;
	sem_post(rc);
	printf("\nReader Inside..%d\n", *((int *)buffer));
	sem_wait(rc);
	--readcount;
	if(readcount == 0)
		sem_post(db);
	sem_post(rc);  
	sleep(3); 

}

