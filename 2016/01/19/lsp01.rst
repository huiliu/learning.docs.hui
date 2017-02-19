Linux系统编程（一） IO基础
*****************************

.. author:: default
.. categories:: program
.. tags:: linux, 系统编程, c
.. comments::
.. more::



术语
=====
*   **API**\ 应该程序编译接口。源代码接口
*   **ABI**\ 应该程序二进制接口。不同架构接口

错误处理
========
定义于\ ``<errno.h>``\ 中的变量\ ``errno``\ 可用于定位错误原因。需要注意的是，\
``errno``\ 仅在短时间内有效，任何后续成功执行的函数都会改写其值。

C库提供了一些函数对\ ``errno``\ 进行包装，可以方便的显示错误文本：
1.  ``perror()``\ 函数原型为：

    .. code-block:: c

        #include <stdio.h>
        void perror(const char *str);

    ``perror``\ 向stderr打印提示：“\ **str:** *errno所描述的错误信息*\ ”。如：

    .. code-block:: c

        if (close(fd) == -1)
            perror("close")

2.  ``strerror()``\ 和\ ``strerror_r()``\ 其原型如下：

    .. code-block:: c

        #include <string.h>
        char *strerror (int errnum);
        int strerror_r (int errnum, char *buf, size_t len);

    ``strerror``\ 返回一个指向错误信息的字符指针，该指针不可被应该程序修改，可\
    以被\ ``perror``, ``strerror``\ 修改。它们都不是安全的。

    ``strerror_r``\ 将错误信息写入到一个长度为\ *len*\ 的字符指针\ *buff*\ 中，\
    成功返回0，失败返回-1。

Tips
-------
1.  对于一些函数，在返回类型的范围内的值都是合法的返回值，在这种情况下使用\
    ``errno``\ 时，在使用前应该清零，在调用后进行检测。如：

    .. code-block:: c

        errno = 0;
        arg = strtoul(buff, NULL, 0);
        if (errno)
            perror("strtoul");

2.  库函数和系统调用都会修改\ ``errno``\ 的值，如果要跨函数来使用保存的\
    ``errno``\ 值，需要先保存该值至变量。如下面的代码就是有问题的：

    .. code-block:: c

        if (fsync(fd) == -1) {
            printf(stderr, "fsync failed!\n");
            if (errno == EIO)
                fprintf(stderr, "I/O error on %d!\n", fd);
        }

    正确的做法是，先保存\ ``error``\ 值：

    .. code-block:: c

        if (fsync(fd) == -1) {
            int err = errno
            printf(stderr, "fsync failed: %s\n", strerror(errno));
            if (err == EIO) {
                fprintf(stderr, "I/O error on %d!\n", fd);
                exit(EXIT_FAILURE);
            }
        }


文件I/O
==========
I/O的基本操作不外乎\ ``读、写、关闭、光标移动``\ 和\ ``截断``\ 等操作，更进一步\
``I/O多路复用``\ 等。内核关键：\ ``虚拟文件系统``\，\ ``页缓存``\ 和\ ``页回写``

内核内幕
=========
1.  虚拟文件系统（VFS）：对底层文件系统进行抽象，提供了统一一致的访问和操作接口。

页缓存及页回写
--------------
1.  **Why?** 物理磁盘访问速度太慢
2.  **How?**


``页缓存``\ 是使用内存中的物理页来缓存磁盘上数据，以减少对访问速度慢的物理磁盘\
的访问。基于\ ``Temporal Locality``\ 理论:刚刚被访问过的资源在短时间内会被再次\
访问。（注：貌似说的是读缓存）

*   **写缓存策略**

    *   不缓存写，直接更新磁盘数据
    *   同时更新缓存数据和磁盘数据
    *   回写。更新缓存，定期将\ ``脏页``\ 回写到磁盘

*   **缓存回收策略**

什么时候回收？怎么回收？ 对干净页进行简单替换。如果干净页不够，则强制回写以腾出\
更多干净页。\ ``最近最少使用，双键策略``\ （貌似在各种使用缓存的地方都会使用LRU\
）。

*   **何时回写**

    *   当空闲内存低于指定阀值时
    *   当脏页在内存中驻留时间超过指定阀值时
    *   用户进程调用\ ``sync()``\ 和\ ``fsync()``\ 系统调用时

相关内核参数位于\ ``/proc/sys/vm``\ 下\ ``dirty_xxx``\ 。更多的还有\
``laptop_mode``\ （那么笔记本有什么特点呢？会有什么特别要求安排呢？）

如何避免拥堵？多线程，回写进（线）程\ ``pdflush (page dirty flush)``\ 的数量。\
拥塞回避策略：主动尝试从没有拥塞的队列回写。

I/O调度
--------
需要了解磁盘的物理结构：磁盘、主轴、磁头。要确定数据在磁盘上的位置，驱动程序需要知道三个值：柱面、磁头、扇区。所以程序从磁盘上读取数据需要不断旋转磁头来查找前面三个值指定的位置。如果不连续，而且序号反复就要花大量时间来查找（寻道）。

I/O调度程序维护一个I/O请求队列，对I/O操作进行\ ``合并``\ 、\ ``排序``\ ，以减少磁盘寻址时间，从而提高全局吞吐量。

*   **I/O调度算法**

    *   Linus电梯
    *   Deadline
    *   预测I/O调度程序
    *   CFQ(Complete Fair Queuing，完全公正队列)
    *   Noop I/O。不进行任何操作。为随机设备而设计(SSD)。

系统启动的时候，可以向内核传递参数，\ ``elevator=as``\ 来设定启用的I/O调度程序。


参考资料
==========
1.  《Linux系统编程》中文版，哈工大
2.  Robert Love著, 陈莉君，康华译，《Linux内核设计与实现》第三版
