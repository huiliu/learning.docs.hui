Linux系统编程（一） IO基础
*****************************

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
        void perror(const char \*str);

    ``perror``\ 向stderr打印提示：“\ **str:** *errno所描述的错误信息*\ ”。如：

    .. code-block:: c

        if (close(fd) == -1)
            perror("close")

2.  ``strerror()``\ 和\ ``strerror_r()``\ 其原型如下：

    .. code-block:: c

        #include <string.h>
        char \*strerror (int errnum);
        int strerror_r (int errnum, char \*buf, size_t len);

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


参考资料
==========
1.  《Linux系统编程》中文版，哈工大
