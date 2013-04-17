Learning Bash
*************

学习Bash，记录一些新学到的知识和tips。

Tips 1 **&&**\ 和\ **||**
==========================
**&&**\ 和\ **||**\ 在条件判断时分别表示“逻辑与”和“逻辑或”，等同于在c中的功能。

另外，它们还可以直接用于命令后面，如：

.. sourcecode:: bash

    command1 && command2 ||command3

上面的意思为：如果命令\ ``command1``\ 正常完成（返回0），则执行\ ``command2``
如果\ ``command2``\ 执行时发生错误（返回非0），则执行\ ``command3``\ 。

如果\ ``commandn``\ 执行多条语句，可以写为\ ``command1; command2; ......``\，如\
果代码为下面的形式：

.. sourcecode:: bash
    
    command1; command2 && command3; command4 || command5; command6

如果期待\ ``command2``\ 正常完成后执行\ ``command3``\ 和\ ``command4``\ ，出错\
时执行命令\ ``command5``\ 和\ ``command6``\ 。那就错了。上面的写法中，命令
``command1``, ``command2``, ``command4``, ``command6``\ 一定会被执行，
``command3``\ ，\ ``command5``\ 则依情况而定。如果期待如前所述，则必须将
``command3; command4``\ 和 ``command5; command6``\ 用"**`**"括起来，如：

.. sourcecode:: bash

    command1; command2 && `command3; command4` || `command5; command6`


Tips 2 关于删除文件时使用引号的疑问
===================================
假定目录结构如下：

|
| tree abc
| .
| ├── a
| └── b
|     └── c
| 
| 2 directories, 1 file

目标是将目录\ *abc*\ 清空。看看下面两个命令，你认为它们的效果是一样么？

.. sourcecode:: bash

    rm -rf "abc/*"
    rm -rf abc/*

动手试试上面两个命令，看看结果如何？第一个命令\ ``rm -rf "abc/\*"``\ 不能删除\
目录\ *abc*\ 下的内容，而第二个命令可以。原因是什么？


Tips 3 防御型代码
==================
今天看《Shell脚本学习指南》p307页时，发现一段代码：

.. sourcecode:: bash

   grep POSTX_OPEN_MAX /dev/null $(find /usr/include -type f | sort)

就觉得奇怪，为什么要加一个\ `/dev/null`\ 在\ `grep`\ 后面。

因为如果\ `$(find /usr/include -type f | sort)`\ 返回为空，\ `grep`\ 命令就会一\
直等待终端输入，导致程序“卡死”。
添加一个\ `/dev/null`\ 的作用就是\ **防止**\ `find`\ **返回为空是程序卡死发生**\ 。
