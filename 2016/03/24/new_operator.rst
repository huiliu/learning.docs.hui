operator new与placement new
************************************
``new`` expression 实际上完成了两项工作：

1.  分配内存
2.  在分配的内存上执行初始化（类的构造函数）
    
可以通过订制\ ``operator new``\ 和\ ``placement new``\ 来控制内存分配。

.. author:: default
.. categories:: c/c++
.. tags:: c/c++, program
.. comments::
.. more::

``new operator``
==================
.. sourcecode:: cpp

    int n = new int(10);

上面的程序片段中，\ ``new``\ ，即：\ ``new operator (expression)``\ 。实际上进\
行了两项工作：

1.  分配一块原始的内存，\ ``operator new``\ 的完成的工作
2.  用\ *10*\ 来初始化第一步分配的内存。(调用对象的构造函数初始化内存)


``operator new``\ 和\ ``placement new``
==========================================
``operator new``\ 用于分配一块原始内存。其功能等同于\ ``malloc``\ ，其形式为：

.. sourcecode:: cpp

    void * operator new(size_t sz);

参数\ ``size_t sz``\ 表示分配内存的多少。

``new operator (expression)``\ 调用\ ``operator new``\ 是隐式传入第一个参数\
``size_t sz``\ 的。同样也可以像调用普通函数一样调用\ ``new operator``\ ，例如：

.. sourcecode:: cpp

    void * buff = operator new(sizeof(std::string));

``operator new``\ 将会返回一块可以容纳\ ``std::string``\ 的原始内存。同样，也\
可以重载\ ``operator new``\ ，但是第一个参数类型必须是\ ``size_t``\ 。事实上， \
``placement new``\ 是\ ``operator new``\ 的一种特定形式，其在标准库中定义为：

.. sourcecode:: cpp

    void * operator new(size_t sz, void * buff)
    {
        return buff;
    }

通过\ ``placement new``\ 可以控制对象存放位置。最简单的，通常\ ``new``\ 产生的\
对象都是在heap上的，但是通过\ ``placement new``\ 可以在stack上\ ``new``\ 新的\
对象。

例如：

.. sourcecode:: cpp

    #include <iostream>
    #include <cstdlib>
    
    class Widget
    {
    public:
        Widget(int32_t n) : m_data(n) {}
        ~Widget() {}
    
        void show()
        {
            std::cout << m_data << std::endl;
        }
    private:
        int32_t     m_data;
    };

    void test()
    {
        // 在stack上构造一个Widget对象
        int value = 1000;
        Widget* pWidget = operator new(&value) Widget(1);

        pWidget->show();

        // 将引发segment fault
        // delete pWidget;
    }

再联想一下，内存的分配、初始化、释放都是要消耗时间的，如果可以将不使用的内存，\
重新回收利用，就能节省分配和释放的时间，对于一些场景是非常具有诱惑力的。如\
memcache, redis等内存数据库。

所以利用\ ``operator new``\ 和\ ``placement new``\ 可以使用一个简单的内存复用\
的内存池。

但是通过\ ``operator new``\ ,\ ``palcement new``\ 来“手动”管理内存，需要程序员\
非常小心，而且，一旦出现问题，可能无法通过第三方工具检测内存泄漏等问题。比较安\
全的一种做法是，一块内存中只保存一种对象类型，而且严禁进行类型转换。

内存释放
==========
对于使用\ ``placement new``\ 产生的对象，应该谨慎使用\ ``delete``\ 来销毁之。\
因为无法确定内存的来源，如果内存是stack上的，使用\ ``delete``\ 会引发\ ``segment
fault``\ (如上面的例子)；如果是在heap上，不会出错。不过为了一致性和安全性，对\
于\ ``palcement new``\ 产生的对象，应该直接调用对象的析构函数，另外销毁原始内\
存。

总结
=====
+-----------------------+---------------------------------------------------+
|   ``new operator``    |   分配内存并初始化(heap-based)                    |
+-----------------------+---------------------------------------------------+
|   ``operator new``:   |   分配一块原始内存(heap)                          |
+-----------------------+---------------------------------------------------+
|   ``placement new``:  |   可以控制内存生成的位置                          |
+-----------------------+---------------------------------------------------+

关于\ ``new/delete``\ 更详细的讨论，请参阅参数资料。

ACKNOWLEGMENT
===============


参数资料
=========
..  [1]     Effective C++(3rd), E16, E49-E52
..  [2]     More Effective C++，E8
..  [3]     C++ Primer(5th, 中文版)
..  [4]     Counting Objects in C++, Scott Meyers, C/C++ Users Journal, April
            1998
