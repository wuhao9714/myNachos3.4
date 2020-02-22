线程机制实习报告
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

本次实验主要考察了对***进程控制块(PCB)的理解***，并通过维护其中较为重要的name，tid，uid以及状态等关键信息，来考察对nachos中***进程相关文件的代码组织结构的理解程度***。

## 内容二：任务完成情况

### 任务完成列表（Y/N）

|          | Exercise1 | Exercise2 | Exercise3 | Exercise4 |
| -------- | --------- | --------- | --------- | --------- |
| 第一部分 | Y         | Y         | Y         | Y         |



### 具体Exercise的完成情况

#### 第一部分

**Exercise1**

***调研Linux或Windows中进程控制块（PCB）的基本实现方式，理解与Nachos的异同。***



我选择Linux，因为其开源，而且我本科期间学过部分相关知识。通过调研Linux-3.5.4，整理如下：

Linux中的每一个进程由一个task\_struct数据结构来描述，也就是常说的进程控制块（PCB），task\_struct存放在/include/linux/sched.h中。其主要参数包括：

-  进程调度信息

> 调度程序利用这些信息决定下一个应该运行的进程。这部分信息包括进程的类别（普通进程还是实时进程）、进程的优先级等。

- 进程状态

> 进程状态是进程调度和交换的依据。Linux下进程设置了5种状态。分别是运行态、可中断态、不可中断态、僵尸状态、暂停态。

- 标识符

> 每个进程都有很多标识符来标识它，标识符有进程标识符、用户标识符、组标识符、备份用户标识符、文件系统用户标识符等。标识符可以用于控制进程对系统中文件和设备的访问。

- 进程通信相关信息

> Linux支持多种不同形式的通信机制。它支持典型的Unix通信机制：信号、管道，也支持System V通信机制：共享内存、信号量和消息队列。

- 时间和定时器信息

> 内核需要记录进程在其生存期内使用CPU的时间以便用于统计、计费等有关操作。进程耗费CPU的时间由两部分组成：一是在用户态下耗费的时间，一是在系统态下耗费的时间。这类信息还包括进程剩余的时间片和定时器信息等，以控制相应事件的触发。

- 文件系统信息

> 进程可以打开或关闭文件，文件属于系统资源，Linux内核要对进程使用文件的情况进行记录。

- 虚拟内存信息

> 除了内核线程，每个进程都拥有自己的地址空间，用mm_struct来描述。

- 页面管理信息

> 当物理内存不足时，Linux内存管理子系统需要把内存中部分页面交换到外存，其交换是以页为单位的。这部分结构记录了交换所用到的信息

- 处理器上下文信息

> 当进程暂时停止运行时，处理机的状态必须保存在进程的task_struct。当进程被调度重新运行时再从中恢复这些环境，也就是恢复这些寄存器和堆栈的值。

- 其他

> 记录一些其他的必要信息。

Nachos相对于Linux来说，Nachos的比较简单，仅定义了几个必须的变量，而且通过阅读源码推断，Nachos的线程没有限制。

**Exercise2**

***仔细阅读下列源代码，理解Nachos现有的线程机制。***

- ***code/threads/main.cc和code/threads/threadtest.cc***

- ***code/threads/thread.h和code/threads/thread.cc***



- main.cc

> main.cc是整个操作系统kernel启动的入口，通过它可以直接调用操作系统的方法。通过程序中的main函数，配以不同的参数，可以调用Nachos操作系统不同部分的各个方法。

- Thread.h

> 定义了管理Thread的数据结构，即Nachos中线程的上下文环境，都被定义在Thread.h中。主要包括当前线程栈顶指针、所有寄存器的状态、栈低、线程状态、名字。还声明了一些基本的方法，如Fork()、Yield()、Sleep()等等。

- Thread.cc

> 实现了Fork()、Finish()、Yield()、Sleep()。
>
> Fork(VoidFunctionPtr func,int arg):func，新线程运行的函数；arg，func函数的参数。它的实现包括一下几步：分配一个堆栈、初始化堆栈、将线程放入就绪队列。
>
> Finish()：并不是直接收回线程的数据结构和堆栈，因为我们仍在这个堆栈上运行这个线程。做法是将threadToBeDestroyed的值设为当前线程，使得Scheduler的Run()可以调用销毁程序，当我们这个程序退出上下文时，将其销毁。
>
> Yield()：调用scheduler找到就绪队列中的下一个线程，并让其执行。以达到放弃CPU的效果

- threadtest.cc

> 主要用于编写测试用例\[1\]。
>
> testnum：测试号，对应相应的测试函数。
>
> SimpleThread()：一个5次循环的程序，每次循环中都让出CPU，让其他就绪的线程执行。
>
> ThreadTest1()：一个测试方法，创建两个线程，让他们都执行SimpleThread()方法，使这两个线程可以交替执行。
>
> ThreadTest()：可以看做一个总控程序，根据main函数传过来testnum参数值来执行不同的测试程序。例如，当testnum==1时，就执行ThreadTest1()。

**Exercise3**

***增加“用户ID、线程ID”两个数据成员，并在Nachos现有的线程管理机制中增加对这两个数据成员的维护机制。***



- 增加用户ID：


> 以当前Linux系统的用户ID作为Nachos的用户ID。
>
> 在thread.h文件的thread类声明中添加私有的int型成员变量userID、添加公有的成员方法getUserID()用于获取当前的userID。
>
> 在thread.cc文件的开始位置，引入头文件“unistd.h”。

- 增加线程ID：


> system.h和system.cc中，定义一个全局的数组int threadIDs\[MaxThread\]，定义这个数组有128个元素，MaxThread为128宏定义，数组的每个元素的取值为0或1。0表示该数组下标没有作为线程的ID分配出去，1表示以分配。allocatedThreadID()：每次创建一个新线程的时候从1开始遍历此数组，得到的第一个值为0元素的数组下标作为线程的ID分配给将要创建的线程。如果遍历一遍数组发现没有可用的ID了，则返回-1.
>
> 在thread.h文件的thread类声明中添加私有的int型成员变量threadID、添加公有的成员方法getThreadID()用于获取当前的threadID。

<img src="..\images\Lab1线程机制\e3-1.png" style="zoom:80%;" />

<img src="..\images\Lab1线程机制\e3-2.png" style="zoom:80%;" />

<img src="..\images\Lab1线程机制\e3-3.png" style="zoom:80%;" />

**Exercise4**

***在Nachos中增加对线程数量的限制，使得Nachos中最多能够同时存在128个线程；***

***仿照Linux中PS命令，增加一个功能TS(Threads Status)，能够显示当前系统中所有线程的信息和状态。***



- 使Nachos最多能同时存在128个线程：


增加一个函数currentThreadNum()获取当前系统中进程数目，在创建线程时，先手动判断当前线程数是否小于128，如果满足条件则创建；否则，不予创建并输出相关信息。

编写Threadtest2()来测试：

<img src="..\images\Lab1线程机制\e4-1.png" style="zoom:80%;" />

<img src="..\images\Lab1线程机制\e4-2.png" style="zoom:80%;" />

<img src="..\images\Lab1线程机制\e4-3.png" style="zoom:80%;" />

- 增加TS功能：


在Thread.cc中添加一个输出本线程的各种信息的函数printThreadInfo()。在system.cc中添加一个数组threadPointers\[MaxThread\]，用于存储已存在的进程的指针；添加一个函数allThreadInfo()利用上述两个函数和数组输出所有进程的信息。

<img src="..\images\Lab1线程机制\e4-4.png" style="zoom:80%;" />

在main.cc中添加如下语句以识别TS.

<img src="..\images\Lab1线程机制\e4-5.png" style="zoom:80%;" />

为了测试上述功能，编写ThreadTest3()以测试Thredtest1()在执行前后，系统中所有线程的信息。

<img src="..\images\Lab1线程机制\e4-6.png" style="zoom:80%;" />

<img src="..\images\Lab1线程机制\e4-7.png" style="zoom:80%;" />

## 内容三：遇到的困难以及解决方法

#### 困难1

发现解决一个问题的方法有很多，常常很难做抉择。例如exercise4中，限定最大线程数。很容易联想到设计模式中的单例模式，因此可以：

<img src="..\images\Lab1线程机制\x1.png" style="zoom:80%;" />

然而，注意到Initialize()中已经使用了Thread()来创建线程，可以预料到在后面实验涉及到的文件中，可能也会有大量使用Thread()来创建线程的地方，如果此处改用新方法，则可能会带来更多的麻烦，所以先暂时不使用这个方法。

除此之外，还可以在分配ThreadID的函数中使用断言，如果进程已满，仍要请求分配新ID，则主动退出程序。但是这种直接结束程序的做法，在以教学为目的的实验中又有些过于绝对，出现错误应该反馈信息并能够继续处理，而不是直接结束程序。

经过长时间思想斗争，最终决定采用最简单的方法，即手动判断。

## 内容四：收获及感想

C/C++语言，本以为自己已经学的不错了，但是仍发现了很多知识盲区，还需要继续努力。

一个好的IDE可以使工作更高效。一开始我使用Ubuntu自带的编辑器，后来改用sublime，再后来使用的VSCODE，发现VSCODE可以自动寻找各变量函数的声明定义位置。

使用了GIT版本工具，在我把程序改崩了的时候可以方便回退。

## 内容五：对课程的意见和建议

虽然说课后实践很重要，但是如果实践之后没有人进行解答疑惑，那实践的效果将大大折扣。建议在提交报告之后的上课结束前10分钟，对已提交实验进行一些讲解。

## 内容六：参考文献

\[1\]佚名 Nachos中文教程

http://wenku.baidu.com/link?url=1rGnypg8Hq6-43gAvuIYPWyVlPLZ0S\_XNEXQJ-2ShqPPg3n2bqWvQgRYC8PdVXLmr66e9GpC2nCSbE1ofkgcT6aASWqVklMWUaBuZNSmXDy
