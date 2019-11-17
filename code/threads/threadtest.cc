// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"
#include "synch.h"
#include <unistd.h>
#define N 5
#define PM 3
#define CM 2
// testnum is set in main.cc
int testnum = 1;

Condition *condc = new Condition("Consumer");
Condition *condp = new Condition("Producer");
Lock *pclock=new Lock("ProducerConsumer");
int count=0;

Semaphore *empty=new Semaphore("empyty",N);
Semaphore *mutex=new Semaphore("mutex",1);
Semaphore *full=new Semaphore("full",0);

Condition *barrCond=new Condition("barrier");
Lock* barrLock=new Lock("barrier");
int currentNum=0;
int barrierNum=5;

int readerCount = 0;
Lock * rLock=new Lock("rLock");
Semaphore * wSema=new Semaphore("wSema",1);
int content=0;

RingBuffer *rbuffer=new RingBuffer(7);
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void 
doNothing(int which){}

void
SimpleThread(int which)
{
    //which 此处代表loop次数
    int num;
    
    for (num = 0; num < which; num++) {
	// printf("*** thread %d looped %d times\n", which, num);
        printf("*** thread name:%s userid:%d threadid:%d looped:%d times \n",
            currentThread->getName(),currentThread->getUserID(),currentThread->getThreadID(),num);
        //currentThread->Yield();
    }
}

void
SimpleThread4(int which)
{
    //which 此处代表loop次数
    int num;
    
    for (num = 0; num < which; num+=10) {
        printf("*** thread name:%s threadid:%d usedtime:%d timeslice:%d\n",
            currentThread->getName(),currentThread->getThreadID(),num+10,currentThread->getUsedTimeSlice()+10);
        currentThread->advanceTime();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

// void
// ThreadTest1()
// {
//     DEBUG('t', "Entering ThreadTest1");

//     Thread *t = new Thread("forked thread");

//     t->Fork(SimpleThread, (void*)1);
//     SimpleThread(0);
// }


void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t1 = new Thread("forked thread1");
    Thread *t2 = new Thread("forked thread2");
  

    t1->Fork(SimpleThread, (void*)5);
    t2->Fork(SimpleThread, (void*)5);

    SimpleThread(5);
}

void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest1");

    printf("*** \"main\" thread userid:%d threadid:%d \n",
            currentThread->getUserID(),currentThread->getThreadID());

    int testThreadNum = 130;
    for(int i=0;i<testThreadNum;i++){
        if(currentThreadNum()<MaxThread){
            Thread *t = new Thread("forked");
            t->Fork(doNothing, (void*)0);
            printf("*** successfully create thread userid:%d threadid:%d \n",
            t->getUserID(),t->getThreadID());
        }
        else{
            printf("The current number of processes has reached the maximum, and the process ID cannot be assigned\n");
        }
    }

}

void
ThreadTest3(){
    printf("this is information at the beginning of ThreadTest1:\n\n");
    allThreadInfo();
    printf("\n");
    ThreadTest1();
    printf("\nthis is information at the ending of ThreadTest1:\n\n");
    allThreadInfo();
    printf("\n");
}

void
ThreadTest4(){
    Thread *t1 = new Thread("thread1");
    Thread *t2 = new Thread("thread2");
    Thread *t3 = new Thread("thread3");
  

    t1->Fork(SimpleThread4, (void*)60);
    t2->Fork(SimpleThread4, (void*)20);
    t3->Fork(SimpleThread4, (void*)40);

    currentThread->advanceTime();
}
void Producer1(int v){
    int times=0;
    while(TRUE){
        pclock->Acquire();
        if(times>=PM){
            if(count>=N)
                condp->Wait(pclock);
            else{
                pclock->Release();
                currentThread->Yield();
                pclock->Acquire();
            }
            times=0;
        }
        while(count>=N){
            printf("Product alread full:%d,threadId:%d,wait consumer.\n",
                count,currentThread->getThreadID());
            condp->Wait(pclock);
            times=0;
        }
        printf("name:%s,threadId:%d,before:%d,after:%d\n", currentThread->getName(),
            currentThread->getThreadID(),count-1,++count);
        times++;
        condc->Signal(pclock);
        pclock->Release();
        sleep(v*0.1);
    }
}
void Consumer1(int v){
    int times=0;
    while(TRUE){
    pclock->Acquire();
    if(times>=CM){
            if(count<=0)
                condc->Wait(pclock);
            else{
                pclock->Release();
                currentThread->Yield();
                pclock->Acquire();
            }
            times=0;
    }
    while(count<=0){
        printf("Product alread empty:%d,threadId:%d,wait producer.\n",
                count,currentThread->getThreadID());
        condc->Wait(pclock);
        times=0;
    }
    printf("name:%s,threadId:%d,before:%d,after:%d\n", currentThread->getName(),
            currentThread->getThreadID(),count+1,--count);
    times++;
    condp->Signal(pclock);
    pclock->Release();
    sleep(v*0.1);
    }   
}
void ThreadPCTest1(){
    Thread *p1=new Thread("Producer1");
    Thread *p2=new Thread("Producer2");
    Thread *c1=new Thread("Consumer1");
    Thread *c2=new Thread("Consumer2");
    p1->Fork(Producer1,1);
    c1->Fork(Consumer1,1);
    p2->Fork(Producer1,1);
    c2->Fork(Consumer1,1);
}
void Producer2(int v){
    while(TRUE){
        empty->P();
        interrupt->OneTick();
        mutex->P();
        interrupt->OneTick();
        printf("name:%s,before:%d,after:%d\n", currentThread->getName(),
            count-1,++count);
        interrupt->OneTick();
        mutex->V();
        interrupt->OneTick();
        full->V();
        interrupt->OneTick();
        sleep(0.5*v);
        interrupt->OneTick();
    }
}
void Consumer2(int v){
    while(TRUE){
    full->P();
    interrupt->OneTick();
    mutex->P();
    interrupt->OneTick();
    printf("name:%s,before:%d,after:%d\n", currentThread->getName(),
            count+1,--count);
    interrupt->OneTick();
    mutex->V();
    interrupt->OneTick();
    empty->V();
    interrupt->OneTick();
    sleep(0.5*v);
    interrupt->OneTick();
    }   
}
void ThreadPCTest2(){
    Thread *p1=new Thread("Producer1");
    Thread *p2=new Thread("Producer2");
    Thread *p3=new Thread("Producer3");
    Thread *c1=new Thread("Consumer1");
    Thread *c2=new Thread("Consumer2");
    p1->Fork(Producer2,1);
    c1->Fork(Consumer2,1);
    p2->Fork(Producer2,1);
    c2->Fork(Consumer2,1);
    p3->Fork(Producer2,1);
}

void barrierFunc(int which){
    barrLock->Acquire();
    currentNum++;
    if(currentNum==barrierNum){
        printf("threadID:%d,currentNum:%d--->broadcast...\n", currentThread->getThreadID(),currentNum);
        barrCond->Broadcast(barrLock);
        barrLock->Release();
    }
    else{
        printf("threadID:%d,currentNum:%d--->wait...\n", currentThread->getThreadID(),currentNum);
        barrCond->Wait(barrLock);
        barrLock->Release();
    }
    printf("threadID:%d continue to run...\n",currentThread->getThreadID());
}

void barrierFunc2(int which){
    for(int i =1;i<=which;i++){
        printf("%s execute %d...\n", currentThread->getName(),i);
        barrLock->Acquire();
        currentNum++;
        if(currentNum<barrierNum){
            barrCond->Wait(barrLock);
        }
        else{
            currentNum=0;
            barrCond->Broadcast(barrLock);
            printf("reach the barrier %d,and enter into next stage...\n",i);
        }
        barrLock->Release();
    }
}

void barrierTest(){
    // for(int i=0;i<barrierNum;i++){
    //     Thread *t=new Thread("test");
    //     t->Fork(barrierFunc,0);
    // }
    barrierNum=3;
    Thread *t1=new Thread("Thread1");
    Thread *t2=new Thread("Thread2");
    Thread *t3=new Thread("Thread3");
    t1->Fork(barrierFunc2,5);
    t2->Fork(barrierFunc2,5);
    t3->Fork(barrierFunc2,5);
}
void read(int n){
    for(int i = 0; i < n; i++){
        interrupt->OneTick();
        rLock->Acquire();
        interrupt->OneTick();
        readerCount++;
        interrupt->OneTick();
        if(readerCount == 1){
            interrupt->OneTick();
            wSema->P();
            interrupt->OneTick();
        }
        interrupt->OneTick();
        rLock->Release();
        interrupt->OneTick();

        printf("readerThread: %s,readed--->%d\n", currentThread->getName(),content);
        interrupt->OneTick();
        rLock->Acquire();
        interrupt->OneTick();
        readerCount--;
        interrupt->OneTick();
        if(readerCount == 0){
            interrupt->OneTick();
            wSema->V();
        }
        interrupt->OneTick();
        rLock->Release();
        interrupt->OneTick();
    }
}
void write(int n){
    for(int i = 0;i < n; i++){
        interrupt->OneTick();
        wSema->P();
        interrupt->OneTick();
        printf("writerThread: %s,write from %d to %d\n", currentThread->getName(),content-1,++content);
        interrupt->OneTick();
        wSema->V();
        interrupt->OneTick();
    }
}

void rwTest(){
    Thread *r1=new Thread("reader1");
    Thread *r2=new Thread("reader2");
    Thread *r3=new Thread("reader3");
    Thread *w1=new Thread("writer1");
    Thread *w2=new Thread("writer2");

    r1->Fork(read,3);
    w1->Fork(write,3);
    r2->Fork(read,3);
    w2->Fork(write,3);
    r3->Fork(read,3);
}
void Producer3(int which){
    // int c=3;
    // int buffer[c];
    // int count=0;
    // int num;
    // interrupt->OneTick();
    // for(int i=0;i<which;i++){
    //     for(int j=0;j<c;j++){
    //         buffer[j]=count++;
    //     }
    //     interrupt->OneTick();
    //     num=rbuffer->put(buffer,c);
    //     if(num==0){
    //         printf("buffer is full,put nothing...\n");
    //         interrupt->OneTick();
    //     }
    //     else{
    //         count=count-(c-num);
    //         printf("put product \n");
    //         for(int j=0;j<num;j++)
    //             printf("%d at buffer [%d]\n",buffer[j],(rbuffer->in-(num-j)+rbuffer->size)%rbuffer->size);
    //         printf("\n");
    //         interrupt->OneTick();
    //     }
    // }
    int buffer[1];
    int count=0;
    int num;
    interrupt->OneTick();
    for(int i=0;i<which;i++){
        buffer[0]=count++;
        num=rbuffer->put(buffer,1);
        if(num==0){
            printf("buffer is full,put nothing...\n");
        }
        else{
            printf("put product %d at buffer [%d]\n",buffer[0],(rbuffer->in-1+rbuffer->size)%rbuffer->size);
        }
        interrupt->OneTick();
        interrupt->OneTick();
        interrupt->OneTick();
    }
}
void Consumer3(int which){
    // int c=2;
    // int buffer[c];
    // int count=0;
    // int num;
    // interrupt->OneTick();
    // for(int i=0;i<which;i++){
    //     for(int j=0;j<c;j++){
    //         buffer[j]=count++;
    //     }
    //     interrupt->OneTick();
    //     num=rbuffer->get(buffer,c);
    //     if(num==0){
    //         printf("buffer is empty,get nothing...\n");
    //         interrupt->OneTick();
    //     }
    //     else{
    //         count=count-(c-num);
    //         printf("get product \n");
    //         for(int j=0;j<num;j++)
    //             printf("%d at buffer [%d]\n",buffer[j],(rbuffer->out-(num-j)+rbuffer->size)%rbuffer->size);
    //         printf("\n");
    //         interrupt->OneTick();
    //     }
    // }
    int buffer[1];
    int count=0;
    int num;
    interrupt->OneTick();
    for(int i=0;i<which;i++){
        buffer[0]=count++;
        num=rbuffer->get(buffer,1);
        if(num==0){
            printf("buffer is full,put nothing...\n");
        }
        else{
            printf("get product %d at buffer [%d]\n",buffer[0],(rbuffer->out-1+rbuffer->size)%rbuffer->size);
        }
        interrupt->OneTick();
        interrupt->OneTick();
        interrupt->OneTick();
    }
}
void kfifoTest(){
    Thread *p=new Thread("producer");
    Thread *c=new Thread("consumer");
    p->Fork(Producer3,12);
    c->Fork(Consumer3,12);
}
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    case 2:
    ThreadTest2();
    break;
    case 3:
    ThreadTest3();
    break;
    case 4:
    ThreadTest4();
    break;
    case 5:
    ThreadPCTest1();
    break;
    case 6:
    ThreadPCTest2();
    break;
    case 7:
    barrierTest();
    break;
    case 8:
    rwTest();
    break;
    case 9:
    kfifoTest();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

