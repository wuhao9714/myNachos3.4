同步机制实习报告
  ------------------

[*内容一：总体概述* ](#内容一总体概述)

[*内容二：任务完成情况* ](#内容二任务完成情况)

[*任务完成列表（Y/N）* ](#任务完成列表yn)

[*具体Exercise的完成情况* ](#具体exercise的完成情况)

[*内容三：遇到的困难以及解决方法* ](#内容三遇到的困难以及解决方法)

[*内容四：收获及感想* ](#内容四收获及感想)

[*内容五：对课程的意见和建议* ](#内容五对课程的意见和建议)

[*内容六：参考文献* ](#内容六参考文献)

## 内容一：总体概述

用信号量来实现锁和条件变量等同步机制，并利用它们实现生产者消费者、读者写者等经典同步互斥问题，用实践来验证理论，深入底层去理解操作系统的同步机制。

## 内容二：任务完成情况

### 任务完成列表（Y/N）

|          | Exercise1 | Exercise2 | Exercise3 | Exercise4 | Challenge1 | Challenge2 | Challenge3 |
| -------- | --------- | --------- | --------- | --------- | ---------- | ---------- | ---------- |
| 第一部分 | Y         | Y         | Y         | Y         | Y          | Y          | Y          |



### 具体Exercise的完成情况

#### 第一部分

**Exercise1 调研**

**调研Linux中实现的同步机制。具体内容见课堂要求。**



Linux内核中同步机制包括原子操作，信号量（semaphore）,读写信号量（rw\_semaphore）,spinlock，BKL(Big Kernel Lock)，rwlock、，brlock（只包含在2.4内核中），RCU（只包含在2.6内核中）和seqlock（只包含在2.6内核中）。

-   原子操作

原子操作需要硬件的支持，因此是架构相关的，其API和原子类型的定义都在内核源码的includ/asm/aatomic.h中，使用汇编代码实现。原子操作主要用于实现资源计数，非常多引用数（refcnt）就是通过原子操作实现的。

```c
typedef struct{

	volatile int counter;

}
atomic_t;
```

volatile字段告诉gcc编译器不要对该数据类型进行优化处理，对他的访问都是对内存的访问，而不是对寄存器的访问。

-   信号量

信号量在创建时要设置一个初始值，表示同时能有几个任务能方法问该信号量保护的共享资源，初始值设为1就变成互斥锁（Mutex），即同时只能有一个任务能访问信号量保护的共享资源。

一个任务要想访问共享资源，该任务必须得到信号量，获取信号量的操作将信号量的值减1，若当前信号量为负数，则表明不能获得信号量，该任务必须挂起在信号量的等待队列等待该信号量可用，若当前信号量的值为非负数，表明能获取信号量，因而可以访问共享资源。

当任务访问完共享资源和后，必须释放信号量，释放信号量通过将信号量的值加一实现。如果信号量的值为非正数，表明有任务等待当前心好累，因此也将唤醒等待队列中的一个任务。

-   读写信号量

读写信号量对访问者进行了细分为读者和写着两种。读者在在保持读写信号量期间只能对读写信号量保护的共享资源进行都方法问，如果一个进程在读的同时还需要进行写操作，那将其划分为写者，他在对共享资源进行访问时必须获得写者身份。读写信号量对同时拥有的读者数不受限制，也就是说可以有多个读者对共享变量进行读操作，而同一时间只能有一个写者进行写操作。

-   自旋锁

自旋锁类似于互斥锁，但是自旋锁不会引起调用者睡眠，如果自旋锁已经被别的执行单位保持，调用者就一直循环等待自旋锁释放。

**Exercise2 源代码阅读**

**阅读下列源代码，理解Nachos现有的同步机制。**

-   **code/threads/synch.h和code/threads/synch.cc**
-   **code/threads/synchlist.h和code/threads/synchlist.cc**



code/threads/synch.h和code/threads/synch.cc：声明和定义了Semaphore，声明Condition和Lock。

> -   Semaphore有一个初值和一个等待队列，提供P、V操作：
>
>     -   P操作：当value等于0时，将当前运行线程放入线程等待队列，当前进程进入睡眠状态，并切换到其他线程运行；当value大于0时，value–。
>
>     -   V操作：如果线程等待队列中有等待该信号量的线程，取出其中一个将其设置成就绪态，准备运行，value++。
>
> -   Lock：Nachos中没有给出锁机制的实现，接口有获得锁(Acquire)和释放锁(Release)，他们都是原子操作。
>
>     -   Acquire：当锁处于BUSY态，进入睡眠状态。当锁处于FREE态，当前进程获得该锁，继续运行。
>
>     -   Release：释放锁（只能由拥有锁的线程才能释放锁），将锁的状态设置为FREE态，如果有其他线程等待该锁，将其中的一个唤醒，进入就绪态。
>
> -   Condition：条件变量同信号量、锁机制不一样，条件变量没值。当一个线程需要的某种条件没有得到满足时，可以将自己作为一个等待条件变量的线程插入所有等待该条件变量的队列，只要条件一旦得到满足，该线程就会被唤醒继续运行。条件变量总是和锁机制一起使。主要接口Wait、Signal、BroadCast，这三个操作必须在当前线程获得一个锁的前提下，而且所有对一个条件变量进行的操作必须建立在同一个锁的前提下。
>
>     -   Wait(Lock \*conditionLock)：线程等待在条件变量上，把线程放入条件变量的等待队列上。
>
>     -   Signal(Lock \*conditionLock)：从条件变量的等待队列中唤醒一个等待该条件变量的线程。
>
>     -   BroadCast(Lock \*conditionLock)：唤醒所有等待该条件变量的线程。
>

code/threads/synchlist.h和code/threads/synchlist.cc：利用锁、条件变量实现的一个消息队列，使多线程达到互斥访问和同步通信的目的，类内有一个Lock和List成员变量。提供了对List的Append()，Remove()和Mapcar()操作。每个操作都要先获得该锁，然后才能对List进行相应的操作。



**Exercise3 实现锁和条件变量**

**可以使用sleep和wakeup两个原语操作（注意屏蔽系统中断），也可以使用Semaphore作为唯一同步原语（不必自己编写开关中断的代码）。**



采用信号量来实现锁和条件变量。

在锁中，要添加一个信号量和一个线程指针，该线程指针用于保存当前拥有锁的线程，Acquire和Release分别执行完P、V操作后，都要记得更新线程指针值。

条件变量中增加一个队列成员变量。Wait操作中，先把当前线程添加到阻塞队列，然后解锁，让出cpu，重新获得cpu后，重新加上锁。Signal从队列中取出一个线程，并将它放入到就绪队列。Broadcast操作使用while循环调用Signal。

在synch.h和synch.cc中添加：

<img src="..\images\Lab3同步机制\e3-1.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e3-2.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e3-3.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e3-4.png" style="zoom:80%;" />



**Exercise4 实现同步互斥实例**

**基于Nachos中的信号量、锁和条件变量，采用两种方式实现同步和互斥机制应用（其中使用条件变量实现同步互斥机制为必选题目）。具体可选择“生产者-消费者问题”、“读者-写者问题”、“哲学家就餐问题”、“睡眠理发师问题”等。（也可选择其他经典的同步互斥问题）**



信号量实现生产者消费者：创建mutex、empty、full三个信号量，初值分别为1、N、0.其实现逻辑如下：

<img src="..\images\Lab3同步机制\e4-1.png" style="zoom:80%;" />

测试程序中，设定生产者消费者各1个，但消费者先进入就绪队列。FIFO调度下，1个生产者，1个消费者，且消费者先入就绪队列，运行结果如下：

<img src="..\images\Lab3同步机制\e4-2.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e4-3.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e4-4.png" style="zoom:80%;" />

随机时间片调度算法下，3个生产者和2个消费者，运行结果如下：

<img src="..\images\Lab3同步机制\e4-5.png" style="zoom:80%;" />



锁和条件变量实现生产者消费者。消费者在没有产品可以消费时，调用condition的wait操作，让出cpu，等待生产者生产。在重新获得CPU后，重新检查当前条件是否能继续消费，若不能，则继续执行wait操作。生产者同理。消费者和生产者使用两个不同的条件变量，一个锁。

测试时，有2个生产者2个消费者，并规定了生产者在一次连续占有CPU期间最多生产3个，而消费者能消费2个，空间大小为5。测试如下：

<img src="..\images\Lab3同步机制\e4-6.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e4-7.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e4-8.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\e4-9.png" style="zoom:80%;" />



**Challenge 1 实现barrier**

**可以使用Nachos 提供的同步互斥机制（如条件变量）来实现barrier，使得当且仅当若干个线程同时到达某一点时方可继续执行。**

使用锁和条件变量，使得当且仅当若干个线程同时到达某一点时方可继续执行。当其中某一线程到达该点时，若条件不满足，则wait；若条件满足，说明该线程是使得条件得到满足的最后一个线程，此时broadcast唤醒其余所有的线程，继续进行下面的运行。

<img src="..\images\Lab3同步机制\c1-1.png" style="zoom:80%;" />

在FIFO调度下，实现简单测试，结果如下：

<img src="..\images\Lab3同步机制\c1-2.png" style="zoom:80%;" />

在随机时间片调度下，创建3个线程，每个线程循环5次输出，条件是进行下一次输出时，必须3个线程都已完成当前输出，实验结果如下：

<img src="..\images\Lab3同步机制\c1-3.png" style="zoom:80%;" />



**Challenge 2 实现read/write lock**

**基于Nachos提供的lock(synch.h和synch.cc)，实现read/write lock。使得若干线程可以同时读取某共享数据区内的数据，但是在某一特定的时刻，只有一个线程可以向该共享数据区写入数据。**

<img src="..\images\Lab3同步机制\c2-1.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\c2-2.png" style="zoom:80%;" />

使用信号量和锁，实现读者优先的读写机制，在随机时间片调度下结果如下：

<img src="..\images\Lab3同步机制\c2-3.png" style="zoom:80%;" />



**Challenge 3 研究Linux的kfifo机制是否可以移植到Nachos上作为一个新的同步模块。**

kfifo的特点是对于读写缓冲区调度in、out管理方式。在研究将kfifo移植到nachos上时，考虑到nachos并不支持多处理器，故将kfifo结构中的自旋锁去掉，并且为了和nachos已经实现的各种同步机制保持统一风格，将对kfifo操作的各函数在nachos中改为kfifo的成员函数。在synch.h和synch.cc中修改：

<img src="..\images\Lab3同步机制\c3-1.png" style="zoom:80%;" />

<img src="..\images\Lab3同步机制\c3-2.png" style="zoom:80%;" />

消费者和生产者各执行12次循环，每次生产和消费1个产品，环形队列长度为5，采用随机时间片调度算法，结果如下：

<img src="..\images\Lab3同步机制\c3-3.png" style="zoom:80%;" />

## 内容三：遇到的困难以及解决方法

#### 困难1

搞清楚随机时间片线程调度花了不少时间。

## 内容四：收获及感想

经过这次lab，除了对上课讲得几种同步机制有了更深的了解外，还为了做challenge3，还花费了大量时间研究kfifo。而且这次为了使测试结果便于观察，有进一步了解了nachos中的时间片等中断机制。

## 内容五：对课程的意见和建议

目前来讲，我觉得一切还都挺好的，没有什么建议。

## 内容六：参考文献

1.*Linux内核同步机制*

> *http://www.51cto.com/html/2006/0322/24177.htm*

2.佚名 Nachos中文教程*

> *http://wenku.baidu.com/link?url=1rGnypg8Hq6-43gAvuIYPWyVlPLZ0S\_XNEXQJ-2ShqPPg3n2bqWvQgRYC8PdVXLmr66e9GpC2nCSbE1ofkgcT6aASWqVklMWUaBuZNSmXDy*
