#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
pthread_mutex_t ra, wa;
int i, j;
int readcount = 0;
int a = 100;

void *reader(void *i)
{
    int num = (int)i;
    printf("\nReader No. %d is trying to read to read the data", num);

    pthread_mutex_lock(&ra);
    readcount++;
    if (readcount == 1)
        pthread_mutex_lock(&wa);

    sleep(1);
    printf("\nReader%d is reading the data %d", num, a);

    pthread_mutex_unlock(&ra);
    printf("\nreader %d has released lock", num);
    readcount--;
    if (readcount == 0)
        pthread_mutex_unlock(&wa);
}

void *writer(void *i)
{
    int num = (int)i;
    printf("\nWriter No. %d trying to write the data", num);
    pthread_mutex_lock(&wa);
    a = a + 1;
    sleep(1);

    printf("\nwriter%d is writing data %d", num, a);

    pthread_mutex_unlock(&wa);
    printf("\nwriter %d has released lock", num);
}

int main()
{
    pthread_t tid[10], tid1[10];
    int nor, now;
    printf("\nEnter the No. readers and writers ");
    scanf("%d%d", &nor, &now);
    pthread_mutex_init(&ra, NULL);
    pthread_mutex_init(&wa, NULL);

    for (i = 0; i < nor; i++)
        pthread_create(&tid[i], NULL, reader, (void *)i + 1);

    for (i = 0; i < now; i++)
        pthread_create(&tid1[i], NULL, writer, (void *)i + 1);

    for (j = 0; j < nor; j++)
        pthread_join(tid[j], NULL);

    for (j = 0; j < now; j++)
        pthread_join(tid1[j], NULL);
}
