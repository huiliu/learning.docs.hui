this指针与const
****************

``this``\ 指针与\ ``const``\ 修饰符的关系。

.. author:: default
.. categories:: c/c++
.. tags:: c/c++, program
.. comments::
.. more::

``const``\ 与指针结合
======================
先看一下\ ``const``\ 指针：

*   *T\* const val;*        不能改变指针的值，可以改变指针指向对象的值
*   *const T\* val;*        不能改变指针指向对象的值，可以改变指针的值（即将指针指向另一个对象）
*   *const T\* const val;*  什么都不能做（实际还是可以做的）

.. sourcecode:: cpp

    void list_const()
    {
        int val1 = 100;
        int val2 = 200;

        // 1. 指针指向的对象是常量，指针可变。即使指向的变量不是常量，指针亦会将
        // 其看作常量
        const int *p1 = &val1;

        // *p1 = 101; // failed
        p1 = &val2;

        // 2. 指针是常量，指针指向的对象可变
        int * const p2 = &val1;

        // p2 = &val2; // failed
        *p2 = val2;

        // 3. 指针和其指向的变量老师常量
        const int* const p3 = &val1;

        /* failed
        p3 = &val2;
        *p3 = val2;
        */
    }

``this``\ 指针
===============
当调用一个类的非静态成员函数时，底层实现会隐式的添加一个指向类实例的指针作为参
数，即\ ``this``\ 指针，\ ``this``\ 指针是一个指针常量（指针的值不可变，其指向\
的对象可变）。

在类（非静态）成员函数中对类的成员变量的访问就是通过\ ``this``\ 指针来完成的，
所以\ ``this``\ 指针的类型影响着对成员变量的操作。

.. sourcecode:: cpp

    class CConst
    {
    public:
        CConst(std::string str) : m_banner(str) {}
        ~CConst() {}
    
        /* 
         *
         * 对于非const成员函数，其this指针类型为T* const，即可以改变成员变量值
         * 对于const成员函数，其this指针类型为const T* const，所以不能改变成员变量的值
         *
         */
        void Print() const
        {
            std::cout << "const member function: " << m_banner << std::endl;
        }

        void Show(/* CConst* const this */)
        {
            std::cout << m_banner << std::endl;
        }
    
        // 对于const成员函数,底层调用时,向其传递的this指针类型为const CConst* const this
        // 所以同名的const成员函数与非const成员函数其实是一种重载
        void Show(/* const CConst* const this */) const
        {
            std::cout << "const member function: " << m_banner << std::endl;
        }
    
    private:
        std::string m_banner;
    };


``const``\ 对象和非\ ``const``\ 对象
======================================
下面两个对象的\ ``this``\ 指针类型是？

.. sourcecode:: cpp

    CConst* pObj = new CConst();    // 非const对象
    const CConst* pConstObj = new CConst();    // 非const对象

如上一节所说，C++底层在调用类的成员函数时会隐式传递一个\ ``this``\ 指针，其类型为：\
``T * const``\ 。所以:

1.  ``pObj``\ 的\ ``this``\ 指针类型为：\ ``CConst * const``
2.  ``pConstObj``\ 的\ ``this``\ 指针类型为：\ ``const CConst * const``

所以：\ ``this``\ 指针是隐式作为参数传递给了成员函数的，而\ ``const``\
对象与非\ ``const``\对象的\ ``this``\ 指针类型是不一样的。所以\ ``const``\
成员函数是一种重载。

联想一下\ ``const``\ 类型的转换规则：

1.  非\ ``const``\ 类型可以转换为\ ``const``\ 类型，反之不行
2.  ``const``\ 指针可以指向非\ ``const``\ 对象，反之不行

所以：\ ``const``\ 对象只能调用const成员函数。

**const成员函数是一种重载；const对象只能调用const成员函数。**

``this``\ 指针的验证
=======================

定义下面一个类：

.. sourcecode:: cpp

    class T
    {
    public:
        T(int n) : m_data(n) {}
        ~T() {}

        void hello() { std::cout << "hello world!" << std::endl; }
        void print() { std::cout << m_data << std::endl; }
    private:
        int m_data;
    };

    void test()
    {
        T * pt = new T(100);
        pt->hello();
        pt->print();    // 打印 100

        delete pt;
        pt = nullptr;
        pt->hello();    // 可以正常输出，因为它没有使用this指针，所以
        pt->print();    // segment fault
    }

上面可以看到：一个对象的\ ``nullptr``\ 指针居然可以正常调用成员函数。
如果不将指针删除后的指针设为\ ``nullptr``\ 会发现什么呢？\
（我用gcc/vc试了好几次，都没有出错，多么恐怖的事啊，一个删除的对象还可以被\
使用，执行结果是未知的啊，出现segment fault是你走大运了，如果正常执行了才麻烦）

C++的对象模型
================
**为什么需要this指针呢？**

ACKNOWLEDGMENT
=================
感谢强哥的指导！
