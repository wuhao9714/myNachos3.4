## 北京大学软件与微电子学院操作系统高级课程Nachos实验

​                                             🥢***如果对您有帮助，请给一个star，感激不尽*🥢**

### **各<u>实验内容</u>与<u>代码分支</u>的对应关系：**

- [Lab1线程机制](./实验内容/Lab1线程机制.md)——[lab1](https://github.com/wuhao9714/myNachos3.4/tree/lab1)
- [Lab2线程调度](./实验内容/Lab2线程调度.md)：
  - 基于优先级的抢占式调度算法——[lab2-priority](https://github.com/wuhao9714/myNachos3.4/tree/lab2-priority)
  - 时间片轮转算法——[lab2-RR](https://github.com/wuhao9714/myNachos3.4/tree/lab2-RR)
  - 多级队列反馈调度算法——[lab2-MLFQ](https://github.com/wuhao9714/myNachos3.4/tree/lab2-MLFQ)
- [Lab3同步机制](./实验内容/Lab3同步机制.md)——[lab3](https://github.com/wuhao9714/myNachos3.4/tree/lab3)
- [Lab4虚拟内存](./实验内容/Lab4虚拟内存.md)——[lab4](https://github.com/wuhao9714/myNachos3.4/tree/lab4)
- [Lab5文件系统](./实验内容/Lab5文件系统.md)——[lab5](https://github.com/wuhao9714/myNachos3.4/tree/lab5)
- [Lab6系统调用](./实验内容/Lab6系统调用.md)——[lab6](https://github.com/wuhao9714/myNachos3.4/tree/lab6)
- [Lab7通信死锁](./实验内容/Lab7通信死锁.md)：
  - 通信——[lab7-communication](https://github.com/wuhao9714/myNachos3.4/tree/lab7-communication)
  - 死锁——[lab7-deadlock](https://github.com/wuhao9714/myNachos3.4/tree/lab7-deadlock)



------

### **实验报告：**

- [Lab1线程机制](./实验报告/Lab1线程机制.md)
- [Lab2线程调度](./实验报告/Lab2线程调度.md)
- [Lab3同步机制](./实验报告/Lab3同步机制.md)
- [Lab4虚拟内存](./实验报告/Lab4虚拟内存.md)
- [Lab5文件系统](./实验报告/Lab5文件系统.md)
- [Lab6系统调用](./实验报告/Lab6系统调用.md)
- [Lab7通信死锁](./实验报告/Lab7通信死锁.md)

> 网络上其它可供参考的实验报告：
>
> - Lab1线程机制
>   - https://blog.csdn.net/wyxpku/article/details/52076236
>   - http://blog.sina.com.cn/s/blog_b9aae6030101mkoa.html
>   - https://blog.csdn.net/superli90/article/details/29369909
>   - https://www.cnblogs.com/icoty23/p/10870568.html
> - Lab2线程调度
>   - https://www.docin.com/p-219256450.html?docfrom=rrela
>   - https://blog.csdn.net/wyxpku/article/details/52076206
>   - https://blog.csdn.net/superli90/article/details/29373593
> - Lab3同步机制
>   - https://blog.csdn.net/wyxpku/article/details/52076209
>   - https://blog.csdn.net/superli90/article/details/29376171
>   - https://icoty.github.io/2019/05/14/nachos-3-4-Lab3/
>   - https://www.docin.com/p-1156828984.html
> - Lab4虚拟内存
>   - https://wenku.baidu.com/view/be56dfe2541810a6f524ccbff121dd36a32dc430
>   - http://blog.sina.com.cn/s/blog_4ae8f77f01018n6r.html
>   - http://blog.sina.com.cn/s/blog_4ae8f77f01018n63.html
>   - https://www.docin.com/p-213029843.html?docfrom=rrela
> - Lab5文件系统
>   - https://wenku.baidu.com/view/04382358f6ec4afe04a1b0717fd5360cbb1a8d40
>   - http://blog.sina.com.cn/s/blog_b9aae6030101mkoe.html
> - Lab6系统调用
>   - https://wenku.baidu.com/view/bd03f40ee97101f69e3143323968011ca300f71e
> - Lab7通信死锁
>   - https://wenku.baidu.com/view/b4c693d1d05abe23482fb4daa58da0116c171f1e



------

### **Nachos3.4问题与答案：**

<u>** **所有问题由陈向群老师提供**</u>

<u>** **所有答案由北京大学软件与微电子学院2019-2020秋季学期《操作系统高级课程》全体同学贡献**</u>

<u>** **在此仅做适当整理**</u>


[<u>*查看答案*</u>](./Nachos问题与答案.md)

1. 新增源代码文件，如何修改makefile
2. 线程管理对应的代码在哪里，Nachos的PCB中管理了哪些信息
3.  nachos线程有几个状态，如何转换？
4. 线程fork的时候，指定了线程要执行的函数，线程在执行这些函数之前做了
5. Thread::Fork 方法中 scheduler->ReadyToRun，前后两行代码有什么作用
6. ThreadRoot()什么时候启动？
7. Nachos中如何销毁线程
8. 线程销毁的方式是否有问题
9. 线程切换时机是什么？线程切换时都做了哪些工作
10. thread.h中Thread类的stacktop和machinestate为什么位置不能换？
11. system.cc中的TimerInterruptHandler函数作用，为什么调用YieldOnReturn()?
12. machine目录下的timer类有什么作用
13. oneTick函数的做了什么？何时调用？
14. nachos的时钟什么时候会前进，如何前进
15. 除了oneTick函数，还有哪个函数修改了stats的ticks？
16. Nachos系统时如何维护线程时间片的？
17. 说清时钟中断的步骤
18. 信号量P操作中有一个while(value==0) 的判断，能否替换为 if(value==0)，原因是?
19. 会不会在Queue中Append多个相同线程
20. Nachos如何保证同步操作的原子性，操作的优缺点。
21. 对信号量p操作关中断后，中断何时会打开？
22. 如何创建user address space？
23. 如何运行user address space中的指令
24. Machine::Run()直到用户线程结束后
25. nachos中什么时候会发生exception？nachos中是如何接收到exception并处理的？
26. 运行用户程序test/sort.c文件到在TLB中
27. 不同的线程, 相同的VPN, 他们的物理页面相同吗?
28. 用户程序不以exit和halt结束会如何
29. 读懂openfile.cc中的ReadAt方法。
30. 文件系统文件内容存放的sector的如何得到的
31. nachos的目录是如何组织的
32. 文件系统初始化的过程
33. create文件的过程
34. remove一个文件的过程
35. nachos系统同步磁盘是怎样实现的？为什么要有这样的同步磁盘机制？如
36. machine类中如何处理syscall
37. 如何获得传递给系统调用的参数？系统调用的返回值是如何传递给调用者
38. 完成系统调用之后如何修改PC值





**友情链接：[朋友的Nachos4.1学习笔记](https://github.com/wsxst/MyNachos4.1)**

