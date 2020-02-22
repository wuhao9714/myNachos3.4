文件系统实习报告
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

在文件系统理论知识的基础上，理解Nachos的文件系统，并完善其功能。

## 内容二：任务完成情况

### 任务完成列表（Y/N）

|          | Exercise1 | Exercise2 | Exercise3 | Exercise4 | Exercise5 |
| -------- | --------- | --------- | --------- | --------- | --------- |
| 第一部分 | Y         | Y         | Y         | Y         | Y         |
| 第二部分 | Y         | Y         |           |           |           |
| 第三部分 | Y         | Y         |           |           |           |

​                    

### 具体Exercise的完成情况

#### 第一部分

**Exercise1 源代码阅读**

Code/filesys/filesys.h:

> 定义文件系统FileSystem类，如果定义了FILESYS宏，就使用Nachos自己实现的版本。
>

Code/filesys/filesys.cc：

> 实现了FileSystem的各种方法，在构造函数中格式化硬盘，0号扇区存放空闲扇区管理结构的FileHeader，1号扇区存放管理根目录的FileHeader。
>

Code/filesys/filehdr.h：

> 定义了文件头FileHeader类，对应Linux里的i-node的概念。
>

Code/filesys/filehdr.cc:

> 实现了分配磁盘空间、读写磁盘数据等功能的函数。
>

Code/filesys/openfile.h：

> 定义打开文件OpenFile类，如果定义了FILESYS宏，就使用Nachos自己实现的版本。
>

Code/filesys/openfile.cc:

> OpenFile::ReadAt和WriteAt函数完成了真正的写入和读取文件数据的操作。
>

Code/filesys/directory.h:

> 定义了目录项DirectoryEntry类和Directory类。
>

Code/filesys/directory.cc:

> 实现了和FileHeader里的两个同名函数不同定义方式FetchFrom和WriteBack。
>

Code/userprog/bitmap.h：

> 定义了BitMap类，用于存储整个系统中位图文件每一位置的状态。
>

Code/userprog/bitmap.cc:

> 实现FetchFrom和WriteBack。
>



**Exercise2 扩展文件属性**

**增加文件描述信息，如“类型”、“创建时间”、“上次访问时间”、“上次修改时间”、“路径”等等。尝试突破文件名长度的限制。**



思路是修改文件头，增加相关信息项成员变量，根据不同的变量做出相应的适配。记得更新直接索引的数量。

<img src="..\images\Lab5文件系统\e2-1.png" style="zoom:80%;" />

**文件类型**:在 filesys/filehdr.h添加变量char type\[4\],用于存储文件的类型。获取类型的方法是，在创建文件时截取文件名中‘.’后面的字符串。同时，修改Print函数，在输出文件头时加上文件类型的输出。

<img src="..\images\Lab5文件系统\e2-2.png" style="zoom:80%;" />

**创建时间，上次访问时间，上次修改时间**：在filesys/filehdr.h定义相关变量和相关函数。创建文件时，更新全部；在读文件的时候，更新上次访问时间；在写文件的时候，更新上次访问和修改时间。

<img src="..\images\Lab5文件系统\e2-3.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e2-4.png" style="zoom:80%;" />

**路径**：目前只有根目录，需要先实现多级目录。

**突破文件名长度限制**，修改directory.h/FileNameMaxLen。



**Exercise3 扩展文件长度**

**改直接索引为间接索引，以突破文件长度不能超过4KB的限制。**



在Exercise2的基础上继续修改。首先将原先的直接索引数组的最后一位改为间接索引。

修改filesys/filehdr.cc中的Allocate函数，如果需要的空间大小大于直接索引，则要考虑间接索引，分配间接索引块并写回硬盘；

<img src="..\images\Lab5文件系统\e3-1.png" style="zoom:80%;" />

修改filesys/filehdr.cc中的Deallocate函数，道理类似。

<img src="..\images\Lab5文件系统\e3-2.png" style="zoom:80%;" />

修改filesys/filehdr.cc中的ByteToSector，根据偏移获得该字节所在的物理块号。特别要注意offset是从0开始的，offset**&lt;**(NumDirect-1)\*SectorSize，是&lt;而不是&lt;=。

<img src="..\images\Lab5文件系统\e3-3.png" style="zoom:80%;" />

修改filesys/filehdr.cc中print函数，包括文件头和文件内容两个部分；

<img src="..\images\Lab5文件系统\e3-4.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e3-5.png" style="zoom:80%;" />

测试：创建大小为1280字节的UNIX文件file3，命令./nachos -cp file3 file3.sh创建了相同内容的nachos文件，./nachos -D查看。1280字节的文件一共需要1280/128=10个物理块，直接索引有8个，间接索引2个。文件系统初始化时已经占用0(freemap的文件头)，1(根目录文件头),2(freemap文件内容)，3、4(根目录内容)；创建file3.sh后，文件头5，直接索引6、7、8、9、10、11、12、13共8个，间接索引块14，间接索引块15、16共2个。

<img src="..\images\Lab5文件系统\e3-6.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e3-7.png" style="zoom:80%;" />



**Exercise4 实现多级目录**



修改filesys/directory.h的DirectoryEntry,添加变量int type表示目录项是文件夹（0）还是文件（1），char path\[20\]表示目录项的绝对路径，使用‘/’分割。

修改filesys/directory.cc，添加FindDir（char \*name），根据绝对路径找到其表示的文件或文件夹所在的上一级文件夹，并返回文件头所在的扇区，如果该绝对路径所表示的不存在，则返回-1；

<img src="..\images\Lab5文件系统\e4-1.png" style="zoom:80%;" />

GetType(char \*name)返回该目录下名字为name的目录项的type，若不存在该目录项则返回-1；

<img src="..\images\Lab5文件系统\e4-2.png" style="zoom:80%;" />

IsEmpty()判断该目录是否为空；

<img src="..\images\Lab5文件系统\e4-3.png" style="zoom:80%;" />

修改Add函数，给定绝对路径，Add由绝对路径所表示的上一级目录调用，在原有基础上，添加目录项时额外增加新的属性；

<img src="..\images\Lab5文件系统\e4-5.png" style="zoom:80%;" />

修改filesys.cc的Create函数，这里要注意的是根据绝对路径创建，所以首先要定位到文件或文件夹的上一级文件夹，其次要从绝对路径中分离出真正的名字；判断在文件夹中是否已存在该名字，若不存在则为其文件头分配扇区。

<img src="..\images\Lab5文件系统\e4-6.png" style="zoom:80%;" />

根据文件和文件夹的不同分别创建；initialsize=-1表示要创建的是文件夹。

<img src="..\images\Lab5文件系统\e4-7.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e4-8.png" style="zoom:80%;" />

修改Open，首先定位到上一级目录，从绝对路径中分离出名字，然后其余和之前相同。

<img src="..\images\Lab5文件系统\e4-9.png" style="zoom:80%;" />

修改remove函数，和create同理，只是在删除时文件夹和文件的处理过程是相同的（文件夹不为空时，不可删除）

<img src="..\images\Lab5文件系统\e4-10.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e4-11.png" style="zoom:80%;" />

测试：增加命令./nachos -cd，fileSystem-&gt;Create(\*(argv + 1),-1);实现文件夹的创建。首先在根目录创建一个test文件夹./nachos -cd test；然后在test下创建file41和file42两个文件./nachos -cp file1 test/file41和./nachos -cp file1 test/file42.（因为在目录项中增加了一些属性，而每个目录能承载的目录项维持不变，所以目录的大小增加了）。

根据结果分析：

> 0 freemap的文件头
>
> 1 根目录的文件头
>
> 2 freemap的内容
>
> 3、4、5、6 根目录的内容
>
> 7 test的文件头
>
> 8、9、10、11 test的内容
>
> 12 test/file41的文件头
>
> 13 test/file41的内容
>
> 14 test/file42的文件头
>
> 15 test/file42的内容

<img src="..\images\Lab5文件系统\e4-12.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e4-13.png" style="zoom:80%;" />

直接删除test失败，因为其非空；

<img src="..\images\Lab5文件系统\e4-14.png" style="zoom:80%;" />

删除掉file41，./nachos -r test/file41；

<img src="..\images\Lab5文件系统\e4-15.png" style="zoom:80%;" />

依此删除file42和test；

<img src="..\images\Lab5文件系统\e4-16.png" style="zoom:80%;" />



**Exercise5 动态调整文件长度**

**对文件的创建操作和写入操作进行适当修改，以使其符合实习要求。**



修改filesys/filehdr.cc，实现函数Extend(BitMap \*freeMap,int bytes)，用于增加文件的长度。获得增长前扇区数量，计算增长后扇区数量，如果增长前扇区数量与增长后扇区数量相同，那么文件不需要额外空间，否则检查剩余空间，如果剩余空间不能满足需求，那么返回错误（false），如果剩余空间能够满足需求，那么进行相关空间的分配。

<img src="..\images\Lab5文件系统\e5-1.png" style="zoom:80%;" />

修改filesys/openfile.cc的函数WriteAt，这个函数的主要功能是从文件特定位置读取特定长度的数据到特定位置，返回值是实际读出的字节数，修改前如果结束位置超过文件长度，那么取文件长度，修改后如果结束位置超过文件长度，那么进行文件长度的增长。

<img src="..\images\Lab5文件系统\e5-2.png" style="zoom:80%;" />

测试：./nachos -t，实际调用了PerformanceTest函数，基本流程是建立TestFile，向文件中循环写入特定的内容（1234567890），再从文件中循环读出，检查内容，删除文件。如果整个过程没有出错，则只输出提示信息。这里设置循环次数为50次。

<img src="..\images\Lab5文件系统\e5-3.png" style="zoom:80%;" />

只输出了必要的信息，没有产生错误。这时查看文件系统，仍能从根目录看到TestFile存在过的痕迹。

<img src="..\images\Lab5文件系统\e5-4.png" style="zoom:80%;" />



#### 第二部分

**Exercise6 源代码阅读**

**a) 阅读Nachos源代码中与异步磁盘相关的代码，理解Nachos系统中异步访问模拟磁盘的工作原理。**

**filesys/synchdisk.h和filesys/synchdisk.cc**



nachos的原始物理磁盘是异步的，发出磁盘请求后会立即返回。当磁盘处理结束后，会通过磁盘中断，来表示任务结束。

为了实现同步磁盘，首先要增加信号量，发出磁盘请求后执行P操作等待，当磁盘处理结束后，通过V操作通知；考虑到nachos是多线程的，每次只能发出一个请求，所以需要用锁来实现互斥。

变量：

```c
Disk *disk;//异步磁盘

Semaphore *semaphore;   //信号量保证条件

Lock *lock;  //锁保证条件
```

函数：

```c
void ReadSector(int sectorNumber, char* data);
//基本功能是读特定扇区,基本流程是获得锁,发出读磁盘相关请求,执行P操作等待磁盘中断，释放锁

void WriteSector(int sectorNumber, char* data);
//基本功能是写特定扇区,基本流程是获得锁,发出写磁盘相关请求,执行P操作等待磁盘中断，释放锁

void RequestDone();
//基本功能是处理磁盘中断，基本流程是执行V操作唤醒读/写操作。
```



**b) 利用异步访问模拟磁盘的工作原理，在Class Console的基础上，实现Class SynchConsole。**

首先观察原始的Console，readAvail-&gt;P()是等待用户往控制台输入字符，当用户输入回车，Console会调用ReadAvail回调（实际上是执行了readAvail-&gt;V()），表示字符串已获取；然后ch = console-&gt;GetChar();取出字符串；console-&gt;PutChar(ch);将字符串写到Console；writeDone-&gt;P() ;等待写完成。

<img src="..\images\Lab5文件系统\e6-1.png" style="zoom:80%;" />

同步控制台，实际上就是把上述操作进行了封装，另外加上锁实现互斥。在userprog/progtest.cc中实现SynchConsole。

<img src="..\images\Lab5文件系统\e6-2.png" style="zoom:80%;" />

ConsoleTest修改为：

<img src="..\images\Lab5文件系统\e6-3.png" style="zoom:80%;" />

测试如下：

<img src="..\images\Lab5文件系统\e6-4.png" style="zoom:80%;" />



**Exercise7 实现文件系统的同步互斥访问机制，达到如下效果：**

**a)  一个文件可以同时被多个线程访问。且每个线程独自打开文件，独自拥有一个当前文件访问位置，彼此间不会互相干扰。**

**b)  所有对文件系统的操作必须是原子操作和序列化的。例如，当一个线程正在修改一个文件，而另一个线程正在读取该文件的内容时，读线程要么读出修改过的文件，要么读出原来的文件，不存在不可预计的中间状态。**

**c)  当某一线程欲删除一个文件，而另外一些线程正在访问该文件时，需保证所有线程关闭了这个文件，该文件才被删除。也就是说，只要还有一个线程打开了这个文件，该文件就不能真正地被删除。**



a)当前open函数是每次打开就新建一个OpenFile对象，可以实现一个文件可以同时被多个线程访问。且每个线程独自打开文件，独自拥有一个当前文件访问位置，彼此间不会互相干扰。

b)首先在开始前先说明一个需要注意的点。在Scheduler::Run中，如果是一个全新的线程（从未获得过CPU），则在SWITCH(oldThread, nextThread);后将直接执行该线程的内容；否则，它将继续执行SWITCH(oldThread, nextThread);后面的全部内容，直到Run结束，才会在上次被中断的位置继续执行。该现象可以用实验验证但具体原因不知，希望有缘人可以告知。

<img src="..\images\Lab5文件系统\e7-1.png" style="zoom:80%;" />

修改filesys/synchdisk.h,增加变量;

<img src="..\images\Lab5文件系统\e7-2.png" style="zoom:80%;" />

修改filesys/synchdisk.cc，实现以下函数；

<img src="..\images\Lab5文件系统\e7-3.png" style="zoom:80%;" />

修改filesys/openfile.cc的Read和Write函数：

<img src="..\images\Lab5文件系统\e7-4.png" style="zoom:80%;" />

在测试前需要明确，同步磁盘SynchDisk的ReadSector和WriteSector的实现机制（通过disk产生磁盘中断来实现同步）决定了，在多线程环境下，多个线程对文件系统的操作将会导致极其繁琐的线程切换。

<img src="..\images\Lab5文件系统\e7-5.png" style="zoom:80%;" />

测试：./nachos -t2调用PerformanceTest2，测试程序如下，

<img src="..\images\Lab5文件系统\e7-6.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e7-7.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e7-8.png" style="zoom:80%;" />

未注释和注释currentThread-&gt;Yield();的结果依次为下图，原因是：一个文件可以在某一时刻有多个打开对象，但是一个OpenFile对象在创建时其包含的文件头hdr的内容就已经确定，在创建后使用前，如果该文件又被打开，且发生了文件大小的动态增长，则之前的OpenFile对象对文件读取的时候就会发生无法读到。所以在给定的测试程序下，读进程能否读到写入的内容，取决于其OpenFile对象创建时，写进程是否写完毕。如果将测试文件的初始大小就定位要写入的字节数，避免文件大小的动态增长，则无论currentThread-&gt;Yield();是否注释掉，读进程都能读到写入的内容，可实验验证（已验证）。更可靠的实现方式是，在上述基础上，在OpenFile的Read和Write中，在ReadAt和WriteAt前更新hdr的内容。

<img src="..\images\Lab5文件系统\e7-9.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e7-10.png" style="zoom:80%;" />

c)记录每一个文件的打开对象个数。

修改filesys/synchdisk.h，增加变量

> int numVisitors\[NumSectors\];     //访问特定文件的线程数量
>

修改filesys/openfile.cc的构造函数，这个函数控制文件的打开过程

> synchDisk-&gt;numVisitors\[sector\]++;
>

修改filesys/openfile.cc的析构函数，这个函数控制文件的关闭过程

> synchDisk-&gt;numVisitors\[sector\]--;
>

修改filesys/filesys.cc的Remove函数，这个函数控制文件删除过程，如果访问当前文件的线程数量不为0，那么当前文件无法被删除

<img src="..\images\Lab5文件系统\e7-11.png" style="zoom:80%;" />

测试：./nachos -t3调用PerformanceTest3，测试程序如下，需要注意的是多线程环境下对文件进行操作，Remove和Write两个操作中实际上会发生多次线程切换。为了测试效果，需要确保删除操作在真正执行时，写操作还未结束，所以在负责写的进程中加了currentThread-&gt;Yield();如果不加，则会出现写操作先结束，导致删除成功。

<img src="..\images\Lab5文件系统\e7-12.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\e7-13.png" style="zoom:80%;" />



#### 第三部分

**Challenge 1 性能优化**

a)  **例如，为了优化寻道时间和旋转延迟时间，可以将同一文件的数据块放置在磁盘同一磁道上**

b)  **使用cache机制减少磁盘访问次数，例如延迟写和预读取。**



a)尽量给同一文件分配连续的空间，修改bitmap.cc，增加Find2函数，寻找连续的空闲区域，若这样的区域存在，那么返回区域起始位置，否则返回-1 ；

<img src="..\images\Lab5文件系统\c1-1.png" style="zoom:80%;" />

修改filehdr.cc的函数Allocate，分配文件空间时先尝试分配连续的空间，如果不能够分配连续的空间，那么按照原来的方法分别分配每个扇区。

<img src="..\images\Lab5文件系统\c1-2.png" style="zoom:80%;" />

测试：首先在Linux下创建file1，file2，file3，大小相当于nachos扇区个数1，3，10.而nachos文件块直接索引8个，一级索引32个。

首先测试直接索引：先创建./nachos -cp file1 file0和./nachos -cp file1 file1，file0占扇区7、8，file1占扇区9、10；删除./nachos -r file0，此时磁盘状态如下：

<img src="..\images\Lab5文件系统\c1-3.png" style="zoom:80%;" />

然后创建./nachos -cp file2 file2，磁盘状态如下：

<img src="..\images\Lab5文件系统\c1-4.png" style="zoom:80%;" />

测试一级索引：先创建./nachos -cp file2 file0和./nachos -cp file2 file1，file0占扇区7、8、9、10，file1占扇区11、12、13、14，删除./nachos -r file0，此时磁盘状态如下：

<img src="..\images\Lab5文件系统\c1-5.png" style="zoom:80%;" />

然后创建./nachos -cp file3 file2，磁盘状态如下：

<img src="..\images\Lab5文件系统\c1-6.png" style="zoom:80%;" />



b)  修改filesys/synchdisk.h，定义cache表项 ，在SynchDisk类中增加Cache \*cache\[CacheNum\];并于构造和析构函数中进行维护

<img src="..\images\Lab5文件系统\c1-7.png" style="zoom:80%;" />

修改filesys/synchdisk.cc的ReadSector函数，首先尝试从cache读取相关数据，如果读取成功，那么更新相关表项的最后访问时间，如果读取失败，那么从磁盘读取相关数据，并且将相关内容写入cache，置换方式是LRU。WriteSector采用write through策略，若cache中包含写扇区，则将其置为无效。

<img src="..\images\Lab5文件系统\c1-8.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\c1-9.png" style="zoom:80%;" />

测试：./nachos -t;修改前，Disk I/O：read 271,write 437;修改后，Disk I/O:read 219,write 437。结构表明，磁盘读次数减少，而由于写采用的是write through，所以不会发生改变。

<img src="..\images\Lab5文件系统\c1-10.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\c1-11.png" style="zoom:80%;" />



**Challenge 2 实现pipe机制**

**重定向openfile的输入输出方式，使得前一进程从控制台读入数据并输出至管道，后一进程从管道读入数据并输出至控制台。**



通过建立管道文件，前一进程从控制台读入数据并输出至管道文件，后一进程从管道文件读入数据并输出至控制台。测试程序如下：./nachos -t4;调用Pipe。主线程负责创建管道文件，另外两个线程分别进行读写。

<img src="..\images\Lab5文件系统\c2-1.png" style="zoom:80%;" />

<img src="..\images\Lab5文件系统\c2-2.png" style="zoom:80%;" />



## 内容三：遇到的困难以及解决方法

#### 困难1

无

## 内容四：收获及感想

无。

## 内容五：对课程的意见和建议

无

## 内容六：参考文献

1.Nachos lab5实习报告.

[*https://wenku.baidu.com/view/04382358f6ec4afe04a1b0717fd5360cbb1a8d40.html*](https://wenku.baidu.com/view/04382358f6ec4afe04a1b0717fd5360cbb1a8d40.html)

2.nachos文件系统

http://blog.sina.com.cn/s/blog_b9aae6030101mkoe.html