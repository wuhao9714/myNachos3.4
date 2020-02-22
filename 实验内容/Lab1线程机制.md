**操作系统实习**

Lab1 线程机制 实习说明

本实习希望通过修改Nachos系统平台的底层源代码，达到“扩展线程机制”的目标。

【实习内容】

**Exercise 1 调研**

调研Linux或Windows中进程控制块（PCB）的基本实现方式，理解与Nachos的异同。

**Exercise 2 源代码阅读**

仔细阅读下列源代码，理解Nachos现有的线程机制。

-   code/threads/main.cc和code/threads/threadtest.cc

-   code/threads/thread.h和code/threads/thread.cc

**Exercise 3 扩展线程的数据结构**

增加“用户ID、线程ID”两个数据成员，并在Nachos现有的线程管理机制中增加对这两个数据成员的维护机制。

**Exercise 4 增加全局线程管理机制**

在Nachos中增加对线程数量的限制，使得Nachos中最多能够同时存在128个线程；

仿照Linux中PS命令，增加一个功能TS(Threads Status)，能够显示当前系统中所有线程的信息和状态。
