static用法
***********

总结关键字\ ``static``\ 的一些用法：

1.  修饰文件中的变量，函数
2.  修饰类的成员函数，成员变量
3.  函数中定义的\ ``static``\ 变量
4.  不同编译单元中的\ ``static``\ 变量的初始化顺序

.. author:: default
.. categories:: none
.. tags:: none
.. comments::
.. more::

控制导出属性
=============
在C/C++中，如果用\ ``static``\ 来修饰一个全局变量，函数，即表示相应的变量和函
数将不被导出，只能在当前编译单元（文件）中使用。如：

.. sourcecode:: cpp

    // file: a.cpp
    static const int s_count = 1000;
    static void func1()
    {
        ///
    }

    void revoke()
    {
        int x = s_count;
        // ......
        func1();
        // ......
    }


    // file: b.cpp

    void say_hello()
    {
        // ......
        // 不能使用到a.cpp中的变量s_count和函数func1 
    }

函数中定义的\ ``static``\ 变量
===============================
局部\ ``static``\ 变量，如：

.. sourcecode:: cpp

    void use_local_static_variable()
    {
        // 这里存在一个潜在的风险
        static int count = 0;
        //...
    }

局部\ ``static``\ 变量，首先是局部变量，所以其作用范围只在声明定义的范围内；\
``static``\ 变量的生命周期是与程序“同生共死”的。局部\ ``static``\ 变量只有在它
所在的代码块被执行的时候才会初始化（与局部变量一样），初始化之后将一直存在，直
到程序结束。


修饰类的成员函数或成员变量
===========================


全局\ ``static``\ 变量的问题
=============================


ACKNOWLEGEMENT
===============


参考资料
=========

