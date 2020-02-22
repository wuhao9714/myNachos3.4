虚拟内存实习报告
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

现有的nachos没有实现虚拟内存，需要在理解TLB机制、页表、中断处理、交换区、反置页表等概念的前提下，将理论知识转换为实际可运行的代码。

## 内容二：任务完成情况

### 任务完成列表（Y/N）

|          | Exercise1 | Exercise2 | Exercise3 |
| -------- | --------- | --------- | --------- |
| 第一部分 | Y         | Y         | Y         |
| 第二部分 | Y         | Y         | Y         |
| 第三部分 | Y         |           |           |
| 第四部分 | N         | Y         |           |

​                             

### 具体Exercise的完成情况

#### 第一部分、TLB异常处理

**Exercise1 源代码阅读**

-   code/userprog/progtest.cc

> 主要定义了加载和执行用户程序的测试操作，同时也包括测试控制台硬件设备的一些函数。最主要的函数为StartProcess。其主要功能是实现用户程序启动，如果希望执行test中的用户程序，则需要切换当前工作目录到userprog，执行./nachos -x ../test/用户程序，通过命令行指令中的-x参数，nachos会在threads/main.cc中调用StartProcess执行用户程序。StartProcess的基本流程是通过文件系统定义的OpenFile打开参数指定文件，通过AddrSpace类构造用户空间，加载文件到内存，通过AddrSpace的InitRegisters函数和RestoreState函数分别完成初始化用户寄存器和装载页表，最后通过machine的Run函数运行用户程序。
>

-   code/machine/machine.h

> 用于描述在Nachos系统中运行用户程序的数据结构。定义内存管理的页大小与磁盘扇区管理的磁盘扇区大小相同均为128Bytes。可用的物理内存页数为32个，因此目前Nachos的内存最大为4KB。定义异常类型的枚举结构和用于存储用户程序指令的寄存器号。
>
> 定义类Instruction，此类定义了系统的指令。对象为指令的二进制表达方式、指令的操作码，指令的三个寄存器的值，指令立即数。操作为将指令的二进制表达方式分析成为系统需要的指令操作码。
>
> 定义类Machine，此类定义了用户程序模拟宿主机的硬件，目的是为了执行用户程序。
>

-   code/machine/machine.cc

> 实现了对寄存器和内存的部分相关操作。其中比较重要的有：
>
> Machine构造函数，将所有系统寄存器内存储值初始化为0。如果使用TLB的话，初始化TLB表，并设置TLB表内的每一个表项有效值为FALSE，设置线性页面转换表为空；如果不使用TLB，设置TLB、线性页面转换表为空。
>
> RaiseException，将出错陷入（Exception）时用户程序的逻辑地址放入r39中；执行一次延迟载入，以结束程序中所有运行这的指令；通过中断调用，设置程序状态为管态；调用异常处理函数ExceptionHandler(which)，将异常处理转入到ExceptionHandler函数中进行处理；通过中断调用，设置程序状态为目态。
>

-   code/machine/ translate.h

> 类TranslationEntry定义了用户程序从逻辑地址转换到物理地址的数据结构线性页面地址转换和TLB页面地址转换，都需要一张地址转换表，地址转换表是由若干个表项（Entry）组成的，TranslationEntry定义的就是每一个表项，每个表项记录程序逻辑页到内存实页的映射关系，和实页的使用状态、访问权限信息。
>

-   code/machine/ translate.cc

> 主要用于将虚拟地址映射到物理地址的操作，定义了ReadMem和WriteMem两个对内存进行读写操作的工具函数，以及重要的Translate函数：给定虚拟地址，转换为物理地址。
>
> 进行判断用户的逻辑地址是否对齐，如果size为4但虚拟地址为3，或size为2而虚拟地址为1，则术语逻辑地址没有对齐，返回用于定义页面访问没有对齐或超出了页面的大小的异常AddressErrorException。确保TLB表或线性页面转换表有且只有一项不为空。通过虚拟地址与页大小相除得到虚拟页表数vpn，通过虚拟地址与页大小相余得到虚拟页表的偏移量offset。如果系统使用的是线性页面转换表，首先判断vpn的大小是否大于等于pageTableSize，如果是，则说明虚拟页数过大，需要返回页面访问没有对齐或超出了页面的大小的异常AddressErrorException；其次判断此vpn所对应的页表项是否有效，如果无效，则返回页面转换出错异常PageFaultException；如果均无问题，得到相应的页表表项。如果系统使用的是TLB转换表，则需要查找TLB表，如果查找到对应表项，则得到相应的页表表项；否则返回页面转换出错异常PageFaultException。如果所得到的相应的页表表项的信息是只读的，而函数参数bool writing值为TRUE，则返回试图访问只读页面异常ReadOnlyException。如果所得到的相应页表表项中的物理地址大于实际现存的物理地址，则返回总线错导致转用户程序页面时出错异常BusErrorException。如果函数参数bool writing值为TRUE，设置表项中的dirty标志，最终返回系统无异常信息NoException。
>

-   code/userprog/ exception.cc

> ExceptionHandler函数：用来处理各类异常。目前除了对关机的处理外其它的还没有实现。
>



**Exercise2 TLB MISS异常处理**

修改ExceptionHandler函数，如果exception为PageFaultException时，添加相应的处理。这里在Machine类中添加一个成员函数SwapTLB，用于将虚拟页添加到tlb中，当tlb中仍有空闲位置时，直接将页表项复制到该位置；如果没有空闲位置，则根据某一置换策略，选择一个位置。同时修改ReadMem和WriteMem函数，在Translate返回PageFaultException的时候，处理完异常后重新调用一次。这里的置换策略和结果演示在exercise3中一起讨论和展示。



**Exercise3 置换算法**

为了实现FIFO和LRU两种调度算法，在TranslationEntry类里添加两个成员enter和lastused，分别用于记录页表项装入tlb和最近一次在tlb中被找到的时间，更新时刻分别为在translate中查找tlb失败而在SwapTLB中装入的时候，和查找tlb成功的时候。

为了统计命中率，在Machine类里添加了tlbhit和tlbunhit两个成员，分别计算命中次数和未命中次数。并且在程序退出时即进程调用SC\_Exit系统调用时，输出该程序的命中相关信息。

FIFO置换算法的应用效果：

<img src="..\images\Lab4虚拟内存\e3-1.png" style="zoom:80%;" />

LRU置换算法的应用效果：

<img src="..\images\Lab4虚拟内存\e3-2.png" style="zoom:80%;" />

可以看出，LRU算法稍优于FIFO置换算法。



#### 第二部分、分页式内存管理

**Exercise4 内存全局管理数据结构**

注意到Userprog/bitmap.cc(h)已经为我们设计好了一个BitMap数据结构，因此将其作为内存全局管理数据结构。首先在Machine类中创建一个bitmap的对象，大小为nachos设定好的最大物理页框数NumPhysPages，并定义一个RequestPage方法，通过bitmap的find方法，返回一个物理页框号。之后，为进程分配物理内存的AddrSpace的构造函数就需要修改，将原先默认的方法改为由RequestPage分配。最后，注意在Machine和AddrSpace类的析构函数中完成bitmap的释放和相关物理页框的释放。

和exercise5一起展示。

**Exercise5 多线程支持**

首先修改AddrSpace中的装载部分代码，因为原先默认只有一个进程，所以采用直接装载方法；而现在可能会有多个进程，每个进程的物理内存空间未必是连续的，所以装载到mainMemory必须使用物理地址。其次，需要把AddrSpace::SaveState函数实现就行了，具体实现为把tlb所有项valid置为FALSE。

为了测试效果，在progtest中创建第二个进程secondthread，运行与main进程相同的文件。main进程先运行，接着通过exit退出，在处理exit退出时，调用该进程的Finish()，这样调度器会运行secondthread。

测试用例和运行结果如下：

<img src="..\images\Lab4虚拟内存\e5-1.png" style="zoom:80%;" />

<img src="..\images\Lab4虚拟内存\e5-2.png" style="zoom:80%;" />



**Exercise6 缺页中断处理**

TLB机制异常引起的PageFaultException,是因为虚拟地址所对应的页表项不在tlb中，在第一部分的exercise2中，默认进程所需要的所有代码和数据全部装入到了内存，且都建立了相应页表，所以此时发生PageFaultException的虚拟页一定可以在该进程的页表中找到对应项，只需要将该页表项复制到tlb的某个位置即可，必要时采用某种置换算法。

而此处的题意，显然为进程所需的代码和数据并没有全部装入内存，即有部分位于硬盘中（即交换区，对应第三部分的Lazy-loading，我们假定该部分是由文件系统创建的一个文件SwapSpace）。则此时如果虚拟页对应的页不位于内存中，则从SwapSpace中将其复制到内存中，如果内存没有空闲页面，则用某置换算法替换已装入内存的某页，如果该页被修改过，还要重新写回SwapSpace。

此处我采用了FIFO的置换算法，在页表项中添加一个调入内存时间的成员变量，每当该页调入内存时，将该变量的值设为此刻的usertick。效果如下图：

<img src="..\images\Lab4虚拟内存\e6-1.png" style="zoom:80%;" />

<img src="..\images\Lab4虚拟内存\e6-2.png" style="zoom:80%;" />

结果解读：可以看到前32个物理页框均在发生了缺页中断时才被分配物理页框，当再次发生缺页中断时，将会按照FIFO的策略进行置换，如vpn-ppn：0-0，1-1，54-2，0、1和52这三个虚拟页最先被加载到内存，分别分配0、1、2三个物理页框，随后36、26、16三个虚拟页将会依此占用0、1、2三个页框。



#### 第三部分、Lazy-loading

**Exercise7**

原先的Address构造函数中已经一次性将文件中的内容加载到内存中，根据上一个exercise6，这里只需将其更改为：创建一个swapspace的文件作为用户文件的交换区，将用户文件加载到swapspace，只有当发生缺页中断时才会将发生了缺页的虚拟页从swapspace中调入内存，而调入的具体页框号则根据此时内存的空闲情况和采用的置换策略而定0。

<img src="..\images\Lab4虚拟内存\e7-1.png" style="zoom:80%;" />



#### 第四部分、Challenges

**Challenge 2**

倒排页表的思想就是整个系统只有一张固定大小的页表，该页表由物理页框号索引，页表项中保存占用该物理页框的进程id和对应的虚拟页号。

因此首先修改AddSpace类的构造函数，将页表的初始化注释掉。在TranslationEntry类中添加tid项，在Machine类中初始化一个反置页表。延续Lazy-loading的思想，当发生缺页中断时，首先在反置页表中遍历，查找是否有满足条件的一项，若查到，则说明进程该虚拟页号已经加载到了物理内存；若没有查到，则调用RequestPage()，若返回结果不为-1，则则修改反置页表对应位置，即将虚拟页号对应的虚拟空间内容从swapspace中加载到刚刚分配的物理页框中；若返回结果为-1，则说明当前物理内存已满，不存在空闲页面，此时使用FIFO的策略置换掉该进程的一页，若被置换的页被修改过，还要写回swapspace。

为了使测试结果便于观察，实现一个输出反置页表当前状态的函数。延续exercise5中的StartProcess，此处只需在部分位置添加输出函数。

<img src="..\images\Lab4虚拟内存\c2-1.png" style="zoom:80%;" />

<img src="..\images\Lab4虚拟内存\c2-2.png" style="zoom:80%;" />

<img src="..\images\Lab4虚拟内存\c2-3.png" style="zoom:80%;" />

<img src="..\images\Lab4虚拟内存\c2-4.png" style="zoom:80%;" />

## 内容三：遇到的困难以及解决方法

#### 困难1

条件编译带来的困扰。因为本次实验涉及TLB的使用与否，而决定nachos是否使用的条件使makefile中是否定义了USE\_TLB。因此一开始想的是在某个文件夹中的makefile修改，但是又因为涉及多个模块，不是很确定哪些模块的makefile需要修改。最后采用的方法是直接在整个code文件夹中进行编译，不需要修改makefile，最后在vm中运行nachos，因为这里的makefile定义了USE\_TLB。

总结问题产生的原因：自己对makefile以及nachos文件结构还不是特别熟悉，导致最后只能用上述这种比较笨的方法。希望在课程末尾，在将所有模块都了解过后，再反过头来把整个nachos再好好捋一捋。

## 内容四：收获及感想

虚拟内存的理论知识自以为已经很了解了，但是一到nachos实现的时候就头疼。我仔细想了想，原因可能有一下几点：

1.  自己对C/C++不是特别熟悉，虽然之前学过，但感觉只是学了点皮毛，现在每次看见比较陌生的C代码，就特别恐惧。

2.  总想着快点做完实验。这就使得我一上来就本着题目去，然后一路磕磕绊绊，到头来还是花了大把时间阅读代码。我觉得以后应该先静下心来，认认真真地去把一定会涉及到的文件读一下，先自己把整个运行流程捋一遍，然后再去根据题目修改。

## 内容五：对课程的意见和建议

希望能抽空讲解一下，虽然说方法没有最好，但是有相对好与坏。在知道“标准答案”后，也能有机会去反思一下，人家的为什么好，自己想出来的办法为什么不是人家的那个，是什么原因导致的？是知识点掌握得不够好还是技巧性不强？

## 内容六：参考文献

*\[1\] https://stackoverflow.com/questions/8843818/what-does-the-fpermissive-flag-do*

*\[2\] http://blog.csdn.net/qyanqing/article/details/9381859 虚拟内存之倒排页表*

*\[3\]* *Nachos中文教程.pdf*
