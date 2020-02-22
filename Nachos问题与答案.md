

1. [新增源代码文件，如何修改makefile](#1新增源代码文件如何修改makefile)
2. [线程管理对应的代码在哪里，Nachos的PCB中管理了哪些信息](#2线程管理对应的代码在哪里nachos的pcb中管理了哪些信息)
3. [nachos线程有几个状态，如何转换？](#3nachos线程有几个状态如何转换)
4. [线程fork的时候，指定了线程要执行的函数，线程在执行这些函数之前做了](#4线程fork的时候指定了线程要执行的函数线程在执行这些函数之前做了)
5. [ Thread::Fork 方法中 scheduler->ReadyToRun，前后两行代码有什么作用 ](#5threadfork-方法中-scheduler-readytorun前后两行代码有什么作用)
6. [ThreadRoot()什么时候启动？](#6threadroot什么时候启动)
7. [Nachos中如何销毁线程](#7nachos中如何销毁线程)
8. [线程销毁的方式是否有问题](#8线程销毁的方式是否有问题)
9. [线程切换时机是什么？线程切换时都做了哪些工作](#9线程切换时机是什么线程切换时都做了哪些工作)
10. [thread.h中Thread类的stacktop和machinestate为什么位置不能换？](#10threadh中thread类的stacktop和machinestate为什么位置不能换)
11. [system.cc中的TimerInterruptHandler函数作用，为什么调用YieldOnReturn()?](#11systemcc中的timerinterrupthandler函数作用为什么调用yieldonreturn)
12. [machine目录下的timer类有什么作用](#12machine目录下的timer类有什么作用)
13. [oneTick函数的做了什么？何时调用？](#13onetick函数的做了什么何时调用)
14. [nachos的时钟什么时候会前进，如何前进](#14nachos的时钟什么时候会前进如何前进)
15. [除了oneTick函数，还有哪个函数修改了stats的ticks？](#15除了onetick函数还有哪个函数修改了stats的ticks)
16. [Nachos系统时如何维护线程时间片的？](#16nachos系统时如何维护线程时间片的)
17. [说清时钟中断的步骤](#17说清时钟中断的步骤)
18. [ 信号量P操作中有一个while(value==0) 的判断，能否替换为 if(value==0)，原因是? ](#18信号量p操作中有一个whilevalue0-的判断能否替换为-ifvalue0原因是)
19. [会不会在Queue中Append多个相同线程](#19会不会在queue中append多个相同线程)
20. [Nachos如何保证同步操作的原子性，操作的优缺点。](#20nachos如何保证同步操作的原子性操作的优缺点)
21. [对信号量p操作关中断后，中断何时会打开？](#21对信号量p操作关中断后中断何时会打开)
22. [如何创建user address space？](#22如何创建user-address-space)
23. [如何运行user address space中的指令](#23如何运行user-address-space中的指令)
24. [Machine::Run()直到用户线程结束后](#24machinerun直到用户线程结束后)
25. [nachos中什么时候会发生exception？nachos中是如何接收到exception并处理的？](#25nachos中什么时候会发生exceptionnachos中是如何接收到exception并处理的)
26. [运行用户程序test/sort.c文件到在TLB中](#26运行用户程序testsortc文件到在tlb中)
27. [不同的线程, 相同的VPN, 他们的物理页面相同吗?](#27不同的线程-相同的vpn-他们的物理页面相同吗)
28. [用户程序不以exit和halt结束会如何](#28用户程序不以exit和halt结束会如何)
29. [读懂openfile.cc中的ReadAt方法。](#29读懂openfilecc中的readat方法)
30. [文件系统文件内容存放的sector的如何得到的](#30文件系统文件内容存放的sector的如何得到的)
31. [nachos的目录是如何组织的](#31nachos的目录是如何组织的)
32. [文件系统初始化的过程](#32文件系统初始化的过程)
33. [create文件的过程](#33create文件的过程)
34. [remove一个文件的过程](#34remove一个文件的过程)
35. [nachos系统同步磁盘是怎样实现的？为什么要有这样的同步磁盘机制？如](#35nachos系统同步磁盘是怎样实现的为什么要有这样的同步磁盘机制如)
36. [machine类中如何处理syscall](#36machine类中如何处理syscall)
37. [如何获得传递给系统调用的参数？系统调用的返回值是如何传递给调用者](#37如何获得传递给系统调用的参数系统调用的返回值是如何传递给调用者)
38. [完成系统调用之后如何修改PC值](#38完成系统调用之后如何修改pc值)



#### 1.新增源代码文件，如何修改makefile

1.增加一个测试的源代码文件：
      假设code/test已有一个新的源文件test1.c，修改同目录下的Makefile：
      （1）all指令后增加新增的源代码文件名称。

```c
  all: halt shell matmult sort test1
```

​      （2）增加编译链接信息。gcc将.c源代码文件编译为.o目标文件，ld将start.o和test1.o组合链接成.coff文件（注意：start.o必须保证放在ld指令后的第一个位置），coff2noff将.coff文件转换为最后的.noff可执行文件。

```c
  test1.o: test1.c
                $(CC) $(CFLAGS) -c test1.c
  test1: test1.o start.o
             $(LD) $(LDFLAGS) start.o test1.o -o test1.coff
             ../bin/coff2noff test1.coff test1
```

2.增加系统中的.h/.cc文件
      假设在machine文件夹下新增一个synchconsole.h/synchconsole.cc，修改一级目录code下的Makefile.common。在如下三处地方添加路径，然后进入code 目录执行make。就能把新增代码编入synchconsole.o用于后续使用。

      USERPROG_H = ../userprog/addrspace.h\
            ……
            ../machine/synchconsole.h\
            ……
            ../userprog/syscall.h
    
      USERPROG_C = ../userprog/addrspace.cc\
            ……
            ../machine/synchconsole.cc\
            ……
           ../machine/translate.cc
    
      USERPROG_O = addrspace.o bitmap.o exception.o progtest.o console.o machine.o \
             mipssim.o translate.o ksyscall.o synchconsole.o
#### 2.线程管理对应的代码在哪里，Nachos的PCB中管理了哪些信息

1.线程管理的对应代码在threads文件夹下的thread.h和thread.cc中；

2.PCB包含了：
（1）线程状态信息--创建、运行、就绪、阻塞
（2）线程用户栈，用于存储用户线程保存的信息。分为int* stackTop、 int* stack分表表示栈顶和栈底。
（3）寄存器状态，保存的是线程相关的上下文信息。
（4）线程名称
（5）用户空间

> 在Nachos实验中，又添加了如下信息
> （1）用户id
> （2）线程id
> （3）线程优先级
> （4）已使用时间片计数

#### 3.nachos线程有几个状态，如何转换？

nachos线程一共有四种状态，分别为：新建态、运行态、就绪态、阻塞态
对应代码在thread.h文件中：

```c
enum ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED };
```

nachos线程状态的转换：

- thread.cc
  - 初始化InitThread()：创建线程，默认为新建态
  - Fork():调用ReadyToRun()方法，由新建态改为就绪态
  - Yield():将当前线程从运行态转为就绪态，并从就绪队列中找到下一个运行线程，并将它转为运行态
  - Sleep():将当前线程从运行态转为阻塞态
  - Finish():设置当前线程为threadToBeDestroyed，然后Sleep()，待下次Run()时销毁线程

- scheduler.cc
  - Scheduler::Run():调度线程上CPU，从就绪态转为运行态

#### 4.线程fork的时候，指定了线程要执行的函数，线程在执行这些函数之前做了

1.调用StackAllocate函数为线程分配栈空间并初始化用户栈指针和寄存器，设置PC寄存器的值为ThreadRoot函数的入口地址，用于线程第一次运行时调用，另外，开中断函数的入口地址、要执行的函数的入口地址及其参数、线程结束调用的函数的入口地址均为ThreadRoot函数的参数，分别存入相应的寄存器中

2.将线程放入就绪队列

#### 5.Thread::Fork 方法中 scheduler->ReadyToRun，前后两行代码有什么作用

分别是关中断和开中断，保证操作的原子性。

1.首先将中断状态设置为IntOff，即关中断；

2.在ReadyToRun函数中，将子线程状态设置为就绪态，并加入调度队列。其中要保证函数的执行不会被其他线程中断，避免出现异常；

3.最后将中断打开，并重置为原中断级别

#### 6.ThreadRoot()什么时候启动？

 新的线程在刚开始运行时会先执行ThreadRoot()函数。线程在调用void Thread::Fork(VoidFunctionPtr func, void *arg)函数的时候，Fork()函数里在调用interrupt->SetLevel(IntOff)关中断之前会调用void Thread::StackAllocate (VoidFunctionPtr func, void *arg)函数为一个新线程分配栈空间。在这个函数里，会将ThreadRoot()的函数指针置于栈顶，同时设置PC指针的值为ThreadRoot()函数指针所在的地址，这样可以保证新的线程在刚开始运行时会先执行ThreadRoot()函数。

该函数的实现是在switch-old.s，ThreadRoot是所有线程的入口，它会调用Fork的两个参数，运行用户指定的函数。

#### 7.Nachos中如何销毁线程

Nachos销毁线程是通过调用Thread类中的Finish操作来完成的。调用Finish后，会先关闭中断，把当前线程标记为将要销毁的线程，然后调用Sleep操作，在Sleep函数中，当前线程会把自身的状态改为阻塞态，并且从就绪队列里面找出新的线程上CPU，如果没有就绪的线程，那么就执行Interrupt中的Idle操作。有的话，让新线程上CPU。新线程如果不是第一次运行的话，就会从上一次被中断的点继续执行，判断是否有线程需要被销毁，如果有的话，就delete该线程的数据结构。

#### 8.线程销毁的方式是否有问题

接着第7题, 线程销毁需要先在Finsh()中其进行赋值. 即threadToBeDestroyed = currentThread. 其中ThreadToBeDestroyed是个指针, 所以这种销毁方式是否有问题? 
结论: 目前好像没发现问题!

讨论一:  如果同时有多个进程调用Finish操作时, 是不是只有最后一个进程的空间会被释放?
解决: 并不是, 因为会有开关中断的限制, 不会有多个进程从执行状态调用Finish()
讨论二: 如果一次性创建多个进程, 直接调用Finish()呢?
解决: 也不行, 因为Finish()中有ASSERT(this == currentThread)语句,判断是否是当前进程在调用

讨论三: 如果讨论二中的新进程始终不进入Run(), 不断申请新进程会导致什么情况?
解决: 进程创建上限为128, 会把进程编号消耗完;

#### 9.线程切换时机是什么？线程切换时都做了哪些工作

线程切换的时机：线程在yield（主动放弃CPU）和 sleep （被阻塞）时会主动调用 scheduler->Run（）方法进行线程切换，finish时会调用sleep方法发生线程切换。

线程切换的工作发生在scheduler.cc中的Run（）方法里。

1.先判断是否是用户线程，如果是的话保存状态。

2.检查要切换下的线程栈是否溢出

3.将调度的新线程设置为currentThread

4.将新线程的状态设置为运行态

5.切换新旧线程。在switch.s里有保存状态的汇编代码。switch 保存旧线程的信息，设置旧线程的PC指令，加载新线程的状态，保存新线程的返回地址。

6.检查旧线程是否是因为执行完毕放弃CPU，如果是要销毁的线程，释放它的空间。

7.再次判断是不是用户线程，是的话加载它的状态。

#### 10.thread.h中Thread类的stacktop和machinestate为什么位置不能换？

主要是因为SWTICH函数。SWITCH函数中是通过基址寻址的方式来获取PCB中的指针地址。

例如_ESP(%eax)，exa寄存器中存放的是pcb的首地址，然后基于这个首地址偏移_ESP个地址获取到栈顶地址，这个_ESP是个宏定义为 0；同理，_EAX(%eax) ，是用用来获取machinestate内存地址的命令，_EAX被定义成4，在PCB中就是stacktop下一个地址，因为statkctop大小为4byte

如果交换位置的话需要将这两个宏定义也互换

#### 11.system.cc中的TimerInterruptHandler函数作用，为什么调用YieldOnReturn()?

system.cc中的initialize函数对nachos全局数据变量初始化，其中初始化timer作为时钟timerinterrupthandler计时器设备的中断处理程序。 定时器设备被设置为周期性地中断CPU（每个TimerTicks一次）。每次有定时器中断（禁用中断）时都会调用此例程。

Yield onreturn设置中断结束后要进行进程切换的标志,必须在中断处理程序中调用，如果yieldOnReturn标志设置，作进程切换,钟中断处理的最后可能会引起进程切换就需要调用该方法

中断返回时，即中断状态由off设置为on时，会调用onetick函数（interupt.cc中定义），ontick对标志进行判断，如果为true就对当前线程进行yield切换，从而保证了等中断处理程序处理完成时，再进行线程的切换

#### 12.machine目录下的timer类有什么作用

timer类的作用模拟硬件计时器的每隔固定的时钟周期就产生一个时钟中断。

它的实现方法是将一个即将发生的时钟中断通过interrupt->Schedule放入中断队列，到了时钟中断应发生的时候，中断系统将处理这个中断，在中断处理的过程中又将下一个即将发生的时钟中断放入中断队列，
这样每隔固定时钟周期，就有一个时钟中断发生。

#### 13.oneTick函数的做了什么？何时调用？

oneTick作用是模拟时钟前进一个单位。

1.根据当前状态为用户态或是系统态时钟分别前进一个用户态时间单位或系统态时间单位，并且对Nachos运行的各项时间（用户态时间、系统态时间）进行统计。

2.检查当前时刻是否有中断发生；如果有，进行中断处理。

3.如果yieldOnReturn标志设置，作进程切换。

Onetick函数在中断重启和一条用户指令被执行后调用。（用户态的时间计算是根据用户指令为单位的；而在系统态，没有办法进行指令的计算，所以将系统态的一次中断调用或其它需要进行时间计算的单位设置为一个固定值，假设为一条用户指令执行时间的10倍。）

#### 14.nachos的时钟什么时候会前进，如何前进

nachos时钟在两种情况下会前进，一个是执行OneTick函数时，另一个是执行CheckIfDue函数且参数advanceClock为True时。

执行OneTick函数会判断系统是在用户态还是在内核态。如果在用户态时钟计数加1，如果在内核态时钟计数加10。OneTick函数会在两种情况下被调用：开启中断时和执行一条用户指令之后。开启中断时，对应的是Interrupt::SetLevel函数。当中断状态从关闭变为打开时，会执行一次OneTick函数。执行一条用户指令后，对应的是mipssim中Machine::Run函数，在for循环中每执行一条用户指令执行一次OneTick函数。CheckIfDue函数的参数advanceClock参数为True时，系统会找到下一个中断发生时间when，并把系统时间前进到when。

#### 15.除了oneTick函数，还有哪个函数修改了stats的ticks？

在interrupt.cc文件中的CheckIfDue函数中进行了修改。函数实现：

1.首先在等待处理的中断队列中取出第一项（最早会发生的中断）。

2.如果不存在任何中断，返回FALSE。

3.如果该中断的发生时机没有到：

- 如果advanceClock没有设置，将取出的中断放回原处，等待将来处理。
- 如果advanceClock设置了，系统时间totalTicks跳到中断将要发生的时间。说明中断马上就要发生。

4.如果当前的状态是Idle态，而且取出的中断是时钟中断，同时等待中断队列中没有其它的中断，意味着系统将退出。但是系统的退出不在这里处理，而是将该中断放回原处，等待以后处理；并返回FALSE。

5.中断发生。inHandler标志设置，说明正在进行中断处理程序。status设置成系统态；很显然，中断处理程序是系统态的。进行中断处理程序的处理，直到处理结束。恢复虚拟机的status和inHandler标志。

#### 16.Nachos系统时如何维护线程时间片的？

在Nachos系统中，线程时间片是通过时钟中断来维护的。

1.Nachos系统中维护了一个中断队列，里面记载着中断的类型、中断时执行的中断处理程序以及中断应该在何时发生。在系统初始化的时候，准确地说是在timer对象初始化的时候，执行Timer类的构造函数Timer::Timer(),该函数会将一个时钟中断插入到中断队列中，时钟中断的间隔由Timer::TimeOfNextInterrupt()计算出。每当用户线程执行一条指令，就会调用Interrupt::OneTick()函数使得时钟向前走,该函数会调用Interrupt::CheckIfDue()函数，CheckIfDue函数的作用是检查中断队列中所有在当前时刻应该发生的中断，并执行相应的中断处理程序。

2.时钟中断在插入到中断队列的时候，指定中断处理函数是TimerHandler,参数是timer对象指针。通过阅读TimerHandler代码可以看到，相当于时钟中断的时候会执行Timer::TimerExpired()函数；TimerExpired函数有两个作用：一是将下一个时钟中断插入到中断等待队列；二是执行(*handler)(arg)。timer对象在初始化的时候指定了handler为TimerInterruptHandler(),而TimerInterruptHandler函数会调用Interrupt::YieldOnReturn()函数来修改Interrupt::yieldOnReturn字段的值为True。简单总结就是在用户线程执行时，时钟中断处理程序会将下一个时钟中断加入到中断等待队列，并将yieldOnReturn值置为True。

3.在OneTick()函数最后会根据yieldOnReturn的值决定当前线程是否要让出CPU，如果yieldOnreturn为True，那么当前线程会执行currentThread->Yield()让出CPU。

#### 17.说清时钟中断的步骤

首先，在system的initial的时候（也就是nachos系统启动时），创建了新的timer（用软件模拟硬件时钟的软件模拟类），这里timer的第一个传入参数即中断处理函数是写好的。（TimeInteruptHandler也就是timer类中的handler）

初始化的时候，使用interrupt->Schedule（向pending队列加入一项）来安排函数，在一定时间后触发第一次中断。这个安排的函数只是TimeExpired，并用一个函数TimeOfNextInterrupt来计算下一次中断到来的时间。（下一次中断到来的时间）。而第一次之后，被TimeExpired触发的中断会在再次schedule 下timeExpired（本身）的同时，执行TimeInteruptHandler并在其中调用yieldOnReturn函数，将yieldOnReturn的布尔值置为1

最后，当CPU转为用户态切回原用户进程后，会先判断yieldOnReturn的布尔值，如果为真则让出CPU使用权，完成中断最后的进程切换。



时钟中断中被schudule的进程的中断触发过程如下：

在用户程序的执行过程中（见mipssims中的one instruct函数），在实行完一条编译命令之后，会调用onetick函数，在使系统时间加上ticks的同时，使用checkIfDue来检查各种类型中断。并最后从pending队列SortedRemove出优先级最高的中断，命名为toOccur，并转为系统态执行toOccur。如果恰好toOccur是时间片中断，那么执行的就是就是前面说到的TimeExpired函数。

#### 18.信号量P操作中有一个while(value==0) 的判断，能否替换为 if(value==0)，原因是?

不能。假设有A，B，C三个进程，有信号量S，value初值为1，A先P操作，value=0，一段时间，A下CPU，没有执行V操作，然后B上CPU，进行P操作被阻塞，A上CPU，之后A进行V操作，value=1，B被唤醒进入就绪队列，假如之后进程C上CPU，执行P操作，value=0，C执行一段时间，下CPU，C还没有进行V操作，然后B上CPU，是if（value==0)的话，B就直接从sleep函数往下执行，进行value--,占用信号量S，但是此时C已经占有信号量S使得value=0,B应该不能占用S的，应该被阻塞，这样就出现了错误。如果是使用while(value==0)的话，B就会再次进行判断，然后被阻塞，不会出错。

#### 19.会不会在Queue中Append多个相同线程

不会。信号量的P会将当前进程Append到信号量的队列中，然后执行sleep操作。在sleep函数中会进行线程的切换，所以进程不会被多次Append到Queue中

#### 20.Nachos如何保证同步操作的原子性，操作的优缺点。

Nachos通过两种⽅式保证同步操作的原⼦性，分别是开关中断和锁。

1.开关中断
        执⾏原⼦操作前关中断，结束操作后打开中断，期间即使有其他中断信号传⼊，也会被屏蔽，开关中断间的操作不被打扰，那么实现了操作的原⼦性。
       优点：简单、⾼效。
       缺点：不适⽤于多处理器系统。
                   需要线程执⾏特权指令，可能被滥⽤，OS⽆法得到控制权。
                   关中断太久可能导致重要的中断请求被丢失。
                   程序的原⼦操作使⽤频繁，执⾏过程时⻓不⼀，若全都采⽤开关中断的⽅式实现原⼦操作，会造成特权指令滥⽤，不易管理。
       因此，在系统中常使⽤锁来实现原子操作。

2.锁
       Nachos中的锁通过信号量实现，当需要原⼦操作前，进程获取锁，操作后释放锁。期间，若有其他进程⼲扰，将会被阻塞进⼊等待队列。
       优点：进程阻塞，不忙等封装开关中断，特权指令不滥⽤。
       缺点：没有开关中断简单⾼效。

#### 21.对信号量p操作关中断后，中断何时会打开？

synch.h中定义了P操作的执行过程

1.首先执行关中断操作。

2.判断value值，若value值大于0则执行第三步，否则令进程进入sleep状态，待被唤醒后执行第2步。

3.执行value--操作并开中断。

#### 22.如何创建user address space？

用户地址空间通过AddrSpace构造函数创建。该构造函数将打开文件作为参数，先利用noffHeader读取文件头部，进行部分运行条件判断：根据魔数判断文件是否为可执行文件；将代码、初始化数据、未初始化数据、用户四者大小相加得出用户地址空间总大小，得出页面数，用以判断是否超出物理页数上线。

在所有条件满足要求后，nachos根据可执行文件大小创建页表，并将可执行文件内容读取至内存中。

#### 23.如何运行user address space中的指令

1.当识别到-x命令时，在main.cc中会调用StartProcess函数运行用户程序

2.StartProcess函数定义在progtest.cc中，将可执行文件加载到内存，创建用户地址空间，然后调用machine->Run()开始执行用户程序

3.machine->Run()函数定义在mipssim.cc中，模拟nachos.上用户程序的执行，当程序开始的时候被内核调用，从不返回。这段代码是可重入的，因为它可以被同时多次调用，然后每个线程执行各自的用户代码:

> 其工作原理为:
> 1.通过 OneInstruction(instr)，将一条指令进行分割，并软件模拟执行。
> 其中，在OneInstruction函数中，通过machine- >ReadMem,读取主存中当前PC值指向的地址里的指令。
> 2.调用onetick让时间前进
> 3.重复1,2

#### 24.Machine::Run()直到用户线程结束后

1.run方法执行在模拟内存的用户程序，反复取指执行译码执行，直到用户程序执行完毕。

2.Nachos通过protest.cc中的StartProcess方法执行文件中的用户程序，目前Nachos中只能在系统内存中运行个用户程序，每一个用户程序在运行之前，Nachos都要初始化一个Addrespace来为新的用户程序分配内存空间，而每一此Addrespace进行初始化时都需要对原有的内存进行归零操作，所以不能同时运行两个用户程序。

3.如果需要运行多个用户程序，需要每个进程初始化自己的用户空间，并且各自执行machine.run才能实现。每行一次machine.run都需要将初始化用户寄存器和载入页表，否则将会执行出错。

#### 25.nachos中什么时候会发生exception？nachos中是如何接收到exception并处理的？

1.exception的类型在machine.h中以枚举的形式进行了定义，共9种：

```c
enum ExceptionType { NoException,           // Everything ok!
       SyscallException,      // A program executed a system call.
       PageFaultException,    // No valid translation found
       ReadOnlyException,     // Write attempted to page marked 
         // "read-only"
       BusErrorException,     // Translation resulted in an 
         // invalid physical address
       AddressErrorException, // Unaligned reference or one that
         // was beyond the end of the
         // address space
       OverflowException,     // Integer overflow in add or sub.
       IllegalInstrException, // Unimplemented or reserved instr.
       

   NumExceptionTypes

};
```

2.nachos运行用户程序时，循环调用machine::oneinstruction取指令、解码、执行指令。在取指令、执行指令的时候如果出现错误（通过检查寄存器发现错误）或者执行系统调用的时候，通过RaiseException函数抛出异常；该函数将出错地址放在寄存器中，然后进入内核态并调用异常处理总入口函数ExceptionHandler；ExceptionHandler根据异常种类或系统调用种类进行相应处理。

（2）nachos运行用户程序时，循环调用machine::oneinstruction取指令、解码、执行指令。在取指令、执行指令的时候如果出现错误（通过检查寄存器发现错误）或者执行系统调用的时候，通过RaiseException函数抛出异常；该函数将出错地址放在寄存器中，然后进入内核态并调用异常处理总入口函数ExceptionHandler；ExceptionHandler根据异常种类或系统调用种类进行相应处理。

#### 26.运行用户程序test/sort.c文件到在TLB中

根据文件名打开文件，然后利用返回的openfile指针创建用户空间，把用户空间加载到当前线程上，关闭文件，
初始化用户寄存器，加载页表，调用machine->run操作，开始执行sort程序的指令，读出PC寄存器的的地址，
并把该地址作为参数，调用machine->ReadMem，执行翻译操作，根据地址检查TLB，产生缺页异常。

#### 27.不同的线程, 相同的VPN, 他们的物理页面相同吗?

相同, 因为Nachos在addrspace.cc中初始化进程空间时, 是一次性把该进程所有内容都复制进内存的, 对于VPN的赋值语句为pageTable[i].virtualPage = i;  即virtual page = phys page. 

#### 28.用户程序不以exit和halt结束会如何

不会怎么样，程序会会正常结束。因为在start.s中定义了用户程序的入口程序__ start，nachos的内核在开始一个用户程序的时候总是会在存放__start函数的location 0位置开始加载，所以当我们从用户程序的main函数返回时，nachos系统自带执行exit(0)，用户程序得到正常结束。

#### 29.读懂openfile.cc中的ReadAt方法。

ReadAt方法包含三个参数（char *into，int numBytes，int position），该函数的功能是从某一存放在磁盘上的文件的position位置开始，将numBytes个字节的数据读入into缓冲区，最后返回实际读出的字节数。首先用两个if判断来检查用户的读请求是否合法，如果请求读取的字节数小于等于0或position大于等于文件长度，则返回0；若position+读取的长度大于文件长度，则将读取长度修改为读取到文件末尾。之后通过divRoundDown函数来计算出开始扇区和结束扇区，这个过程就是简单的用字节数去除以SectorSize的过程，之后for循环一次读入一个扇区，最后返回实际读取的字节数。

#### 30.文件系统文件内容存放的sector的如何得到的

1.从磁盘载入文件的目录文件 

2.在文件目录中，通过文件名，查找文件头部对应的sector号(通过directory的find(name)函数,find函数会调用findindex函数找到该文件在目录项表table中的表项序号，根据序号去目录表找到对应的目录项，目录项里面放着sector)

#### 31.nachos的目录是如何组织的

Nachos 在其模拟器上实现了文件系统。 

实现的主要功能有：创建删除文件，对文件进行读写操作，建立从逻辑结构到物理磁盘之间的映射。可以进行按名存取。在nachos中有i节点，i节点不是存放在一起的，而是通过bitmap来进行管理空间的i节点，一个i节点对应虚拟磁盘中的一个扇区，所以是使用bitmap来管理磁盘上的空闲块。 

Nachos中位图文件中存放的是整个文件系统的扇区使用的位图的情况。该文件存放的是整个文件系统的扇区使用情况的位图。如果一个扇区为空闲，则它在位图文件相应的位为0，否则为1。

Nachos中没有专门对inode扇区进行管理。 当需要申请一个扇区时，根据位图文件寻找一个空闲的扇区，并将其相应的位置为1。当释放一个扇区时，将位图中相应的位置为0。位图文件是一个临界资源，应该互斥访问。

现有的文件系统没有实现互斥访问，所以每次只允许一个线程访问文件系统。位图文件的inode占据0号扇区。 在文件系统的目录管理方面。Nachos只有一级目录，也就是只有根目录，所有的文件都在根目录下。而且根目录中可以存放的文件数是有限的。Nachos文件系统的根目录同样也是通过文件方式存放的，它的inode占据了1号扇区。 文件的索引结构上。

Nachos采用索引表进行物理地址和逻辑地址之间的转换，索引表存放在文件的i节点中。但是目前Nachos采用的索引都是直接索引，所以Nachos的最大文件长度不能大于4K。 必须在文件生成时创建索引表。所以Nachos在创建一个文件时，必须给出文件的大小；而且当文件生成后，就不能改变文件的大小。目前文件系统的健壮性不够强。当正在使用文件系统时，如果突然系统中断，文件系统中的内容可能不保证正确。

#### 32.文件系统初始化的过程

文件系统初始化的过程定义在filesys.cc中filesystem类构造函数中，fileSystem对象实在System.cc 文件中创建的。实现：如果format设置为false，则使用原有的文件系统打开位图文件和目录文件，返回；如果format标志设置为ture，则生成一个新的文件系统，步骤如下：

1.     生成新的位图和空白的根目录
2.     生成位图FileHeader和目录FileHeader
3.     在位图中标识0和1号扇区被占用（虽然此时还没有占用）
4.     为位图文件和目录文件申请必要的空间，如果申请不到，系统出错返回
5.     将位图FileHeader和目录FileHeader写回0和1号扇区（这时候位图文件和目录文件所在的位置已经确定）
6.     打开位图文件和目录文件
7.     将当前的位图和目录写入相应的文件中（位置确定，内容终于可以写入）而且这两个文件保持打开状态

#### 33.create文件的过程

create文件的入口是FileSystem::Create函数，该函数会获取到文件目录和磁盘空间位图，若新文件名与目录中的文件不重复，则会尝试为文件的文件头分配空间，若成功，会尝试在文件目录中添加文件目录项，若成功则再为文件数据分配空间，如果空间分配成功则说明新文件创建成功，然后把文件头，文件目录和位图写回磁盘。

为文件分配空间调用的是FileHeader::Allocate函数，该函数会利用磁盘空间位图为文件寻找和分配空间。

#### 34.remove一个文件的过程

在根目录下搜寻该文件名
1. 如果没有搜索到，返回FALSE
2. 如果搜索到，打开该文件并返回打开文件控制块，并读入位图
1. 根据文件头，释放文件数据块扇区 
2. 释放FileHeader所占用的空间 
3. 删除根目录中该文件目录 
4. 将对位图和目录的修改写回磁盘

#### 35.nachos系统同步磁盘是怎样实现的？为什么要有这样的同步磁盘机制？如

Nachos中同步磁盘的机制是保证线程对磁盘访问的互斥性。

在Nachos只是一个模拟的操作系统，并不能对物理磁盘进行操作，所以Nachos系统的模拟磁盘其实是宿主机上的一个名为“DISK”的文件。

Disk类将“DISK”文件抽象成了一个磁盘，并向上层提供了两个接口：ReadRequest(int sectorNumber, char* data)和void WriteRequest(int sectorNumber, char* data);分别用于发出度读磁盘的请求和写磁盘的请求。通过阅读源代码可以看到，在ReadRequest和WriteRequest中，其实读写磁盘的过程完成之后，将一个中断加入到了中断等待队列中。

阅读源代码可以看到，在ReadRequest和WriteRequest返回之后，会执行semaphore->P()阻塞。其实这是和ReadRequest和WriteRequest中的注册中断过程是配套的，认真阅读代码可以发现磁盘中断主要做的事情其实就是semaphore->V()，这是模拟现实中磁盘I/O期间，发出请求的进程会阻塞，当I/O完成之后发出信号唤醒进程的过程。

但是Disk类并没有实现对磁盘的互斥访问，如果不加一定的机制，上述过程可能会出现一定的问题。我们考虑一种情形：A线程在完成写磁盘操作之后，阻塞在信号量P上，此时另一个线程B也能对磁盘进行修改，则可能将A线程写的内容给覆盖掉，这样就很容易出错。所以必须保证磁盘读写的互斥性。

同步磁盘syndisk其实就是给磁盘加了一把锁，将Disk::ReadRequest和Disk::WriteRequest进行了封装，在调用者两个函数前必须先抢锁，拿到锁之后才能对磁盘进行修改；而锁的释放操作则放在了semaphore->(）之后，也就是必须一个磁盘请求完成之后才能处理下一个磁盘请求。这样就避免了由于对磁盘访问不互斥带来的问题。

#### 36.machine类中如何处理syscall

1.machine->Run()开始运行用户程序

2.machine->OneInstruction(Instruction *instr)处理一条指令，首先通过PC读取指令，之后对指令进行decode，根据instr->opCode分类处理

3.通过switch语句可以看出当OP_SYSCALL时，会直接调用RaiseException(SyscallException, 0)之后return

4.RaiseException会设置当前状态为内核态，并且进入异常处理程序ExceptionHandler

5.ExceptionHandler定义在exception.cc中，判断异常类型，发现类型是系统调用时，读取寄存器，从2号寄存器中得到系统调用号，进行相应系统调用的处理。

6.可以看出在系统调用处理时，PC是不改变的，当处理完系统调用，OneInstruction会直接返回，重新执行当前语句，因此对于大部分系统调用，需要设置新的PC以避免重复执行。

#### 37.如何获得传递给系统调用的参数？系统调用的返回值是如何传递给调用者

可以通过访问machine的registers获得系统调用的参数，系统调用号在r2寄存器，第一个参数arg1在r4寄存器，arg2在r5寄存器，arg3在r6寄存器，arg4在r7寄存器。系统调用的返回值保存在r2寄存器，可以通过读r2寄存器获取返回值。

#### 38.完成系统调用之后如何修改PC值

系统调用完成后应该执行用户程序的下一条指令，所以应该将PC值加4指向下一条指令继续执行。在Nachos中与PC有关的有三个寄存器：PrevPCReg、PCReg、NextPCReg，所以应该将prevPC置为PC,PC加4，NextPC为PC加4
