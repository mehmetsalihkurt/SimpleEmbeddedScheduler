
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS	5



typedef unsigned long (* mThread)(void);

void push_thread(int thread_num,long timeToProcess);

enum
{
    stop = 0,
    ms1 = 0x01,
    mn1 = ms1 * 60,
    hr1 = mn1 * 60,
};

enum 
{
    th1,
    th2,
    th3,
    th4,
    lastThread
};

typedef struct  
{
    unsigned int number;
    mThread thread;
    unsigned long processTime;
}mThreadQueueT;

unsigned long myThread1(void)
{
    printf("hello from thread 1\n");
    return stop;
}

unsigned long myThread2(void)
{
    push_thread(th1,2);
    printf("hello from thread 2\n");
    return 5;
}

unsigned long myThread3(void)
{
    
    return 2*ms1;
}

unsigned long myThread4(void)
{
    
    return stop;
}


mThreadQueueT threads[lastThread] = 
{
    {th1,&myThread1,0},
    {th2,&myThread2,0},
    {th3,&myThread3,0},
    {th4,&myThread4,0},
};

static int systemTime = 1;
void *onems(void *threadid)
{
   
   while(1)
   {
       systemTime++;
       printf("system time %d \n",systemTime);
       sleep(1);
   }
   pthread_exit(NULL);
}

void push_thread(int thread_num,long timeToProcess)
{
    if(thread_num < lastThread)
        threads[thread_num].processTime = systemTime + timeToProcess;
}

int main()
{
    pthread_t orgthreads[NUM_THREADS];
   int rc;
   long t;
   int queueCnt = 0;
   
    rc = pthread_create(&orgthreads[0], NULL, onems, (void *)t);
    
    push_thread(th2,7);
    
    while(1)
    {
       //printf("system time %d \n",systemTime);
        //printf("processTime %d \n",threads[queueCnt].processTime);
        if((systemTime >= threads[queueCnt].processTime) && (threads[queueCnt].processTime != 0))
        {
            unsigned long time = threads[queueCnt].thread();
            
            if(time)
                threads[queueCnt].processTime = systemTime + time;
            else
                threads[queueCnt].processTime = time;
        }
        
        queueCnt = (queueCnt+1)%lastThread;
        usleep(1000);
    }
    pthread_exit(NULL);
    
    return 0;
    
}
