系统调用实习报告
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

在虚拟内存实验的基础上，理解nachos的详细系统调用过程，实现常见的系统调用，并能编写用户程序测试各系统调用。

## 内容二：任务完成情况

### 任务完成列表（Y/N）

|          | Exercise1 | Exercise2 |
| -------- | --------- | --------- |
| 第一部分 | Y         |           |
| 第二部分 | Y         | Y         |
| 第三部分 | Y         | Y         |



### 具体Exercise的完成情况

**一、理解Nachos系统调用**

**Exercise 1 源代码阅读**

**阅读与系统调用相关的源代码，理解系统调用的实现原理。**

- **code/userprog/syscall.h**
- **code/userprog/exception.cc**
- **code/test/start.s**



**code/userprog/syscall.h:** 定义了nachos的系统调用，包括系统调用号和系统调用函数的声明。包括已经实现了的Halt()在内的系统调用一共有11个。其余未实现的10个分别是：

> 涉及地址空间的：1. void Exit(int status);用户程序结束时调用，status=0表示正常退出。2. SpaceId Exec(char \*name);加载执行名字是name的Nachos可执行文件，返回地址空间ID。3. int Join(SpaceId id);等待标识符是id的用户线程执行完毕，返回退出状态。
>
> 涉及文件系统的：1. void Create(char \*name);创建一个nachos文件，其名为name。2. OpenFileId Open(char \*name);打开nachos文件name，返回其打开文件ID。3. void Write(char \*buffer, int size, OpenFileId id);从buffer中写size字节到打开文件ID为id的文件。4. int Read(char \*buffer, int size, OpenFileId id);从打开文件读取size字节到buffer。5. void Close(OpenFileId id);关闭打开文件ID为id的文件。
>
> 涉及用户级线程的：1. void Fork(void (\*func)());创建运行func函数的线程，其与当前线程拥有相同地址空间。2. void Yield();让出CPU。
>

**code/userprog/exception.c:** 定义了异常处理函数void ExceptionHandler(ExceptionType which)，根据which的不同，执行不同的处理流程。

**code/test/start.s:** 辅助用户程序运行的汇编语言。\_\_start函数调用了用户程序的main函数。如果main函数返回了，就调用Exit函数退出当前进程。系统调用部分是将的调用号存放到r2寄存器中，然后通过syscall指令陷入内核，交给内核处理。



**二、文件系统相关的系统调用**

**Exercise 2 系统调用实现**

**类比Halt的实现，完成与文件系统相关的系统调用：Create, Open，Close，Write，Read。Syscall.h文件中有这些系统调用基本说明。**



首先要编写PC指针加1的操作，因为上述系统调用成功后，用户程序需要执行的是系统调用的下一条指令。因为非系统调用指令在OneInstruction()中会一直执行到函数末尾，完成PC值的更新，而如果该指令是系统调用OP\_SYSCALL，则会在处理完系统调用后直接return，下一次OneInstruction时由于PC值未更新，则会出错。因此，在编写系统调用时，必须要完成PC值的更新。

<img src="..\images\Lab6系统调用\e2-1.png" style="zoom:80%;" />

创建文件系统调用，通过在r4寄存器中找到文件名字的内存地址，通过ReadMem获得文件名，再用文件系统已经实现的Create函数创建文件。

<img src="..\images\Lab6系统调用\e2-2.png" style="zoom:80%;" />

与创建文件类似，打开文件系统调用在获得文件名后调用Open，同时将打开文件指针存入r2寄存器作为返回值。

<img src="..\images\Lab6系统调用\e2-3.png" style="zoom:80%;" />

关闭文件只需要delete打开文件指针。

<img src="..\images\Lab6系统调用\e2-4.png" style="zoom:80%;" />

读文件首先从三个寄存器中读取参数，然后调用文件系统的Read，将指定大小的内容读入到临时变量，然后再写回内存对应的位置，最后将实际读取字节数作为返回值存入r2.

<img src="..\images\Lab6系统调用\e2-5.png" style="zoom:80%;" />

写文件首先从三个寄存器中读取参数，然后从内存指定位置读取指定大小的内容到临时变量，再将其写入指定文件。这里需要注意的是，如果文件是控制台，则在此处输出内容。

<img src="..\images\Lab6系统调用\e2-6.png" style="zoom:80%;" />



**Exercise 3 编写用户程序**

**编写并运行用户程序，调用练习2中所写系统调用，测试其正确性。**



编写如下测试程序。同时修改test中的makefile。控制台打印I love nachos3.4，证明程序正确。

<img src="..\images\Lab6系统调用\e3.png" style="zoom:80%;" />



**三、执行用户程序相关的系统调用**

**Exercise 4 系统调用实现**

**实现如下系统调用：Exec，Fork，Yield，Join，Exit。Syscall.h文件中有这些系统调用基本说明。**



对于Exec，创建一个新的Thread类同时它还要有独立的地址空间，具体实现如下。

<img src="..\images\Lab6系统调用\e4-1.png" style="zoom:80%;" />

<img src="..\images\Lab6系统调用\e4-2.png" style="zoom:80%;" />

Join函数循环判断等待的线程是否已被销毁，如果销毁，则跳出；否则，让出CPU。

<img src="..\images\Lab6系统调用\e4-3.png" style="zoom:80%;" />

Fork函数的参数为函数指针，其实是用户程序地址空间中函数的虚拟地址。创建一个线程，运行相同的用户程序，不同的是，fork线程是从指定位置运行的。因此，在为fork线程Fork的函数中，必须初始化PC为给定函数指针。

<img src="..\images\Lab6系统调用\e4-4.png" style="zoom:80%;" />

<img src="..\images\Lab6系统调用\e4-5.png" style="zoom:80%;" />

Yield也只需要简单调用线程Thread的Yield函数。

<img src="..\images\Lab6系统调用\e4-6.png" style="zoom:80%;" />

Exit调用currentThread-&gt;Finish()方法。

<img src="..\images\Lab6系统调用\e4-7.png" style="zoom:80%;" />



**Exercise 5 编写用户程序**

**编写并运行用户程序，调用练习4中所写系统调用，测试其正确性。**



测试程序如下：

<img src="..\images\Lab6系统调用\e5-1.png" style="zoom:80%;" />

也可以将本次实验的所有系统调用一次性测试，只需要Exec执行的是文件部分测试程序即可：

<img src="..\images\Lab6系统调用\e5-2.png" style="zoom:80%;" />



## 内容三：遇到的困难以及解决方法

#### 困难1

自己之前一直都是在上一个作业的基础上，进行本次作业的。但是这一次遇到了好多的错误，例如地址空间和段错误等待。因为之前的作业已经很久了，而且有些地方修改得我都有点混乱了。这次花了好大的功夫才修改过来，不过也算是当作nachos的复习了。

## 内容四：收获及感想

如果可以有下一次，我觉得应该每次作业的每一个execise都做一个备份，这样当有错误的时候也方便查错。

## 内容五：对课程的意见和建议

无。

## 内容六：参考文献

1.Nachos lab6实习报告.

https://wenku.baidu.com/view/bd03f40ee97101f69e3143323968011ca300f71e

2.nachoswenjian系统

http://blog.sina.com.cn/s/blog_b9aae6030101mkoe.html
