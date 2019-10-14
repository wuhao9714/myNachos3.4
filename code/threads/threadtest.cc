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

// testnum is set in main.cc
int testnum = 1;

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
    allThreadInfo();
    for (num = 0; num < which; num++) {
	// printf("*** thread %d looped %d times\n", which, num);
        printf("*** thread name:%s userid:%d threadid:%d looped:%d times \n",
            currentThread->getName(),currentThread->getUserID(),currentThread->getThreadID(),num);
        currentThread->Yield();
    }
}

void
SimpleThread2(int which)
{
    int num;
    printf("*** thread name:%s priority:%d userid:%d threadid:%d looped:%d times \n",
            currentThread->getName(),currentThread->getPriority(),currentThread->getUserID(),currentThread->getThreadID(),0);
    if(which>=2){
        Thread *t1 = new Thread("fork",which);
        t1->Fork(SimpleThread2, (void*)which-3);
    }
    for (num = 1; num < 5; num++) {
    // printf("*** thread %d looped %d times\n", which, num);
        printf("*** thread name:%s priority:%d userid:%d threadid:%d looped:%d times \n",
            currentThread->getName(),currentThread->getPriority(),currentThread->getUserID(),currentThread->getThreadID(),num);
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
ThreadTest4()
{
    DEBUG('t', "Entering ThreadTest4");

    SimpleThread2(5);
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
    default:
	printf("No test specified.\n");
	break;
    }
}

