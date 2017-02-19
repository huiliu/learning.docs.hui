由内联函数引起的一个BUG
************************



.. author:: default
.. categories:: c/c++
.. tags:: c/c++, program, debug, tips, learn
.. comments::
.. more::



假定有三个文件inline.h, inline.cpp, main.cpp，内容如下：

``inline.h``\ 类CPeople定义如下，其中包含一个内联函数\ ``SetHouse``\ 。

.. literalinclude:: _static/code/inline_problems/inline.h
    :language: cpp
    :linenos:
    :emphasize-lines: 10,15

``inline.cpp``\ 类CPeople的实现。成员函数\ ``Show``\ 仅仅是方便调试下断点。

.. literalinclude:: _static/code/inline_problems/inline.cpp
    :language: cpp
    :linenos:
    :emphasize-lines: 20

``main.cpp``\ 中创建一个实例，并调用被实现为内联的成员函数\ ``SetHouse``

.. literalinclude:: _static/code/inline_problems/main.cpp
    :language: cpp
    :linenos:
    :emphasize-lines: 7


``Makefile``\ 是这样的：（请特别注意Makefile）

.. literalinclude:: _static/code/inline_problems/Makefile
    :language: makefile
    :linenos:
    :emphasize-lines: 10,7


首先我们运行\ ``make``\ 命令编译代码：

.. code-block:: shell
   :linenos:

   make
   g++ -g -std=c++11 inline.cpp -c -o inline.o
   g++ -g -std=c++11   -c -o main.o main.cpp
   g++ -g -std=c++11 inline.o main.o -o main

请注意上面make的输出，编译得到了\ `inline.o`\ 和\ `main.o`\ ，然后将两者链接得
到\ `main`\。

运行\ `main`\ 在"``inline.cpp``"的20行放置断点，那么成员变量的值分别为？用GDB
看一下实际输出：

.. code-block:: text

    (gdb) p \*this
    $1 = {m_age = 10, m_hasHouse = true, m_pNull = 0x0}

你答对了吗？

接下来，将类CPeople中的变量\ ``m_hasCar``\ 注释去掉，再编译：

.. code-block:: shell
   :linenos:

   make
   g++ -g -std=c++11 inline.cpp -c -o inline.o
   g++ -g -std=c++11 inline.o main.o -o main

看仔细了，\ ``make``\ 的输出有什么问题吗？

再用\ ``GDB``\ 调试main，同样在\ ``inline.cpp``\ 的第20行放置断点，看看此时的
成员变量值：

.. code-block:: text

   (gdb) p \*this
   $1 = {m_age = 10, m_hasCar = true, m_hasHouse = false, m_pNull = 0x0}

你发现问题了吗？

成员函数\ ``SetHouse``\ 修改的应该是\ ``m_hasHouse``\ 的值，结果却是\
``m_hasCar``\ 的值变了。出现BUG呢！！！

这是为什么呢？
