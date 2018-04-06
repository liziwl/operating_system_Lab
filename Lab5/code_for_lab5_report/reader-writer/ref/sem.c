#include<stdio.h>  
#include<semaphore.h>  
#include<fcntl.h>  
#include<unistd.h>  
#include<sys/stat.h>  
#include<sys/types.h>  
  
void printer(sem_t *sem, char *str)  
{  
    sem_wait(sem);  //信号量减一  
    while(*str!='\0')  
    {  
        putchar(*str);    
        fflush(stdout);  
        str++;  
        sleep(1);  
    }  
    printf("\n");   
      
    sem_post(sem);  //信号量加一  
}  
  
int main(int argc, char *argv[])  
{  
    pid_t pid;  
    sem_t *sem = NULL;  
      
    pid = fork(); //创建进程  
    if(pid<0){ //出错  
        perror("fork error");  
          
    }else if(pid == 0){ //子进程  
      
        //跟open()打开方式很相似,不同进程只要名字一样，那么打开的就是同一个有名信号量  
        sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 1); //信号量值为 1  
        if(sem == SEM_FAILED){ //有名信号量创建失败  
            perror("sem_open");  
            return -1;  
        }  
          
        char *str1 = "hello";  
        printer(sem, str1); //打印  
          
        sem_close(sem); //关闭有名信号量  
          
        _exit(1);  
    }else if(pid > 0){ //父进程  
          
        //跟open()打开方式很相似,不同进程只要名字一样，那么打开的就是同一个有名信号量  
        sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 1); //信号量值为 1  
        if(sem == SEM_FAILED){//有名信号量创建失败  
            perror("sem_open");  
            return -1;  
        }  
          
        char *str2 = "world";  
        printer(sem, str2); //打印  
          
        sem_close(sem); //关闭有名信号量  
          
        wait(pid, NULL); //等待子进程结束  
    }  
      
    sem_unlink("name_sem");//删除有名信号量  
      
    return 0;  
}  