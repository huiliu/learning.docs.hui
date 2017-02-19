const修饰符
**************

const修饰符的特殊使用方法。\ [1]_ [2]_

.. author:: default
.. categories:: c/c++
.. tags:: c/c++, program
.. comments::
.. more::

例一 经典的\ ``char* pchar = "xxx"``
==========================================
C中最常见的一个问题-\ ``char* pchar = "xxx"``\ 的隐含了什么，算是C语言的一个坑，也算是语法要求不严谨。如下：

.. sourcecode:: cpp

    void testConstChar()
    {
        char* pchar = "abc";
        pchar[1] = 'a';
    }


**函数\ ``testConstChar``\ 执行结果是什么样的？为什么？**
从字面上看\ ``char* pchar = "abc";``\ 只是定义子一个指向字符串的指针；而实际
上\ ``pchar``\ 的类型应该是: ``const static char*``\ 。即：\ ``pchar``\ 隐含
了\ ``const``\ 属性，修改pchar指向的内存必然引发'segment fault'。

在C编译器中完全没有任何提示警告；C++编译器会警告：\ `deprecated conversion
from string constant to ‘char*’`

另外如果将上面的变量\ `pchar`\ 声明为: `char pchar[] = "abc";`\ 则表示\
`pchar`\ 是一个字符数组。

例二 对返回值的影响
====================
对例一进行一下扩展延伸，定义下面这样一个类及其成员函数：

.. sourcecode:: cpp

    class cTestConstChar
    {
    public:
        const char* GetChar()
        {
            char* pChar = "hello world!";
            return pChar;
        }
    };

    void testConst()
    {
        cTestConstChar c;
        std::cout << c.GetChar() << std::endl;
    }


**上面的函数运行结果是怎么样的？为什么？**

一眼看去\ ``cTestConstChar::GetChar()``\ 返回了一个局部指针变量，将引发未知\
结果。

回想一下\ *例一*\ ，可以知道，局部变量\ `pChar`\ 实际类型是: ``const static
char*``\ ，即：它指向的内存区域不会因为函数返回而被释放，所以\
``cTestConstChar::GetChar()``\ 将会返回一个正确可用的，指向字符串\
"hello world!"的地址。将会正常输出"hello world!"。

.. note::

    再推广一下：函数可以返回局部的\ ``static``\ 变量的地址或引用。（但是好像没有
    人推荐这种用法:)）


例三 修改\ ``const``\ 变量值
================================
``const``\ 修饰的变量，其值是不可以修改的，但是可以通过方法去掉\ ``const``\ 属
性，这样就可以修改了，看看下面这个例子的：


.. sourcecode:: cpp

    void testChangeConstValue()
    {
        const int nValue = 10;
        int* pInt = const_cast<int*>(&nValue);
    
        *pInt = 100;
        std::cout << *pInt << ", " << nValue << std::endl;
    
    }

**函数\ ``testChangeConstValue``\ 的输出会是什么？为什么？**

|   结果：100, 10

为什么？
1.   常量的值不可改变？那么const_cast是干什么用的？
2.   const_cast分配的一块新的内存空间来保存nValue的值？
3.   ...

通过断点逐步调试看看

调试会发现: ``pInt``\ 确实指向了\ *nValue*\ 的内存地址，而且修改了内存值，那么
为什么\ *nValue*\ 的输入结果依然是10呢？

推测应该是编译器在编译时直接将nValue替换为了10, 在调用cout时没有去读内存值。查看
汇编代码可以确认。

.. note::

    非绝对必要，还要修改\ ``const``\ 变量的值，即使修改也要仔细考虑，调整代码，
    注意编译器的优化。 

例四 类的\ ``const``\ 成员函数
===============================
类的\ ``const成员函数``\ 不能修改类的成员变量，看一下下面这个例子：

.. sourcecode:: cpp

    class cTestConstMember
    {
    public:
        cTestConstMember();
        ~cTestConstMember();
    
        void DoThings() const;
    
    private:
        int         m_nCount;
        mutable int m_nMutable;
        int*        m_pInt;
    };
    
    cTestConstMember::cTestConstMember()
    : m_nCount(10)
    , m_nMutable(1)
    , m_pInt(nullptr)
    {
        m_pInt = new int(100);
    }
    
    cTestConstMember::~cTestConstMember()
    {
        if (nullptr != m_pInt) {
            delete m_pInt;
            m_pInt = nullptr;
        }
    }
    
    void cTestConstMember::DoThings() const
    {
        std::cout << "Test Class Const Member Function" << std::endl;
        std::cout << m_nCount << ", " << m_nMutable << ", " << *m_pInt << std::endl;
    
        // 妄图修改类成员变量，编译都无法通过
        // m_nCount = 100;
        m_nMutable = 11;
        *m_pInt = 10;
    
        std::cout << m_nCount << ", " << m_nMutable << ", " << *m_pInt << std::endl;
    }
    
    void TestConstMember()
    {
        /*const */cTestConstMember c;
        c.DoThings();
    }

这个例子就不细说了，在上面使用了关键字\ ``mutable``\ ，\ `mutable`\ 修饰的变量
不受对象的\ ``const``\ 修饰符的影响，可以被修改。

参考资料
==========

.. [1]  `cv (const and volatile) type qualifiers
        <http://en.cppreference.com/w/cpp/language/cv>`_
.. [2]  `const (C++) <https://msdn.microsoft.com/en-us/library/07x6b05d.aspx>`_
