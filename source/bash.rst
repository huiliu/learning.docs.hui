Learning Bash
*************

学习Bash，记录一些新学到的知识和tips。

Tips 1 **&&**\ 和\ **||**
=====================
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


