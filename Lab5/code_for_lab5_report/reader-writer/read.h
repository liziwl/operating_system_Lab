#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include<semaphore.h>
#include<pthread.h>


extern sem_t *rc, *db;
extern int readcount;

void *reader(int *buffer) {
    sem_wait(rc);
    if (0 == readcount) {
        sem_wait(db);
        readcount++;
    }
    sem_post(rc);
    printf("\nReader Inside..%d\n", *buffer);
    sem_wait(rc);
    readcount--;
    if (0 == readcount) {
        sem_post(db);
    }
    sem_post(rc);
}
