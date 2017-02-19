# -*- encoding: utf-8 -*-
DLL与LIB的创建与使用
**********************
记录分别Visuall Studio 2015和GCC创建和使用动态库与静态库的\
方法。

.. author:: default
.. categories:: c/c++
.. tags:: c/c++, program
.. comments::
.. more::


VS 2015创建动态库和静态库
==========================
直接使用向导即可创建动态库和静态库

VS 2015中使用动态库和静态库
=============================

库与应用程序在同一个解决方案
-----------------------------
只需要在相应的项目中添加对动\静态库项目的引用即可(当然头文件包含路径需要设置)

使用第三方提供的库
-------------------
如果使用的第三方法编译好的库，通常应该包含头文件和库文件两部分。对于动态库dll,\
一般会包含一个符号文件.lib文件。

首先要添加头文件(.h)的引用路径。然后在.c/cpp文件中使用指令\
"``#program comment(lib, "lib file")``"来加载库文件。

.. note::

    需要注意的是应用程序的"**运行库**"(工程属性->C/C++->代码生成)方式必须与库一致，否则无法通过编译链接。

Linux下创建静态库和动态库
===========================
静态库
---------
在Linux下创建静态库文件极为简单：

1.  将.c/cpp编译为.o文件

    .. code-block:: shell

        gcc -c \*.c

2.  使用命令"``ar``"将.o文件打包为静态库

    .. code-block:: shell

        ar -crs libb.a *.o

    *-c*    创建存档文件
    *-r*    将.o文件插入存档文件
    *-s*    为存档文件创建索引

存档文件的顺序非常重要，\ **最佳实践是将链接库文件放在链接命令的最后面。**

.. note::

    链接时，链接器会从左到右查找引用符号，如果.o目标引用了它前面(左边)库中的符\
    号，链接器会找不到此符号。

动态库
--------
Linux下要创建动态库，直接加上编译参数"``-shared -fPIC``"即可：

.. code-block:: shell

    gcc -shared -fPIC -o libapi.so *.c

默认会导出动态库的所有符号。使用编译参数"``-fvisibility=hidden``"可以隐藏所有\
导出。另外GCC支持"*导出映射*"功能，可以显示的定义动态库中对客户可见的符号，导\
出映射文件的格式为：(export.map)

.. code-block:: text

    {
        global: DoSomething;
        local: *
    }

编译时，通过编译参数"``-version-script``"指定映射文件：

.. code-block:: shell

    gcc -shared -fPIC -o libapi.so *.c -version-script=export.map

还可以将静态库文件解开，重新生成动态库：

.. code-block:: shell

    ar -x libapi.a

    gcc -shared -o libapi.so *.o


Linux下使用静态库和动态库
===========================
gcc通过参数"``-lapi``"来指定连接库的名字；"``-Lpath``"来添加库的搜索路径。默认
链接动态库，"``-static``"选项告知链接器要静态链接。

默认链接动态库：

.. code-block:: shell

    gcc -o a.out hello.c -L . -lapi

静态链接：

.. code-block:: shell

    gcc -o a.out hello.c -L . -lapi -static


libtool
=========
Linux下可以使用工具"**libtool**"方便的创建库文件，可执行文件。

.. code-block:: shell

    libtool -static -o libapi.a *.o

    libtool -shared -o libapi.so *.o



参数资料
=========

1.  `演练：创建和使用动态链接库 (C++) <https://msdn.microsoft.com/zh-cn/library/ms235636.aspx>`_
2.  `演练：创建和使用静态库 (C++) <https://msdn.microsoft.com/zh-cn/library/ms235627.aspx>`_
3.  `指令pragma <https://msdn.microsoft.com/en-us/library/d9x1s805.aspx>`_
4.  `#pragma comment(...) <https://msdn.microsoft.com/en-us/library/7f0aews7.aspx>`_
5.  `在Visual Studio中使用C++创建和使用DLL <http://www.jellythink.com/archives/111>`_
6.  《C++ API设计》，附录A
