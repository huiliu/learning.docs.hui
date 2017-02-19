字节码解释器，虚拟机简单原型
===================================

制件自己的编程语言之基础，字节码解释器，也是一个mini虚拟机。

.. author:: default
.. categories:: none
.. tags:: none
.. comments::
.. more::

《游戏编程模式》一书中有一章讲字节码（模式），自定义一些指令集，对应于游戏中的\
某些操作，由用户（不是指游戏玩家）来自由组合使用它们灵活实现一些功能。这种功能\
等同于提供接口给脚本使用，方式上类似于实现一个小型的虚拟机。

在这里重制一个超简单的虚拟机。

操作码(opcode)
===============
首先确定虚拟机有哪些操作，需要的指令集（有点类似于汇编指令）。
通常都会有：
*   运算操作（加减乘除等）
*   比较运算
*   逻辑运算
*   条件语句
*   循环语句
*   switch语句
*   函数调用

在这里只定义加减乘除操作

.. code-block:: c

    enum Instruction
    {
        OPCODE_NUM,     // 数值(操作数)
        OPCODE_ADD,     // add op
        OPCODE_SUB,     // sub op
        OPCODE_DIV,     // div op
        OPCODE_MUL,     // mul op
    }; 

操作数
=======
有了上面的操作，总得有操作对象吧，这就是操作数了，也算是函数的参数吧。这些操作\
数是如何保存的呢？栈。

字节码解释器
=============
字节码解释器，即不断读取字节码，并解释执行之。

.. code-block:: c

    class VM
    {
    public:
        VM() : StackIndex_(0) {}
    public:
        char interpret(char bytecode[], size_t size);

    private:
        int add(int left, int right);
        int sub(int left, int right);
        int div(int left, int right);
        int mul(int left, int right);

    private:
        char    pop();          // 出栈
        void    push(char c);   // 入栈
    private:
        static const char MAX_STACK = 100;
        char StackIndex_;
        char Stack_[MAX_STACK];             // 存入操作数的栈
    };


    char VM::interpret(char bytecode[], size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            // 提取操作码
            Instruction opcode = static_cast<Instruction>(bytecode[i]);
            switch (opcode)
            {
            case OPCODE_NUM:
                push(bytecode[++i]);    // 将操作数压入栈
                break;
            case OPCODE_ADD:
            {
                char a = pop();     // 从栈中取值（操作数）
                char b = pop();
                char value = a + b;
                push(value);        // 将运算结果压入栈
            }
                break;
            case OPCODE_SUB:
            {
                char a = pop();
                char b = pop();
                char value = a - b;
                push(value);
            }
                break;
            case OPCODE_DIV:
            {
                char a = pop();
                char b = pop();
                char value = a / b;
                push(value);
            }
                break;
            case OPCODE_MUL:
            {
                char a = pop();
                char b = pop();
                char value = a * b;
                push(value);
            }
                break;
            default:
                assert(false);
                break;
            }
        }

        return pop();
    }


上面使用字节码的方式实现了一个解释器，具有加减乘除计算器的功能。

小结
====
上面的实现类似实现一套自定义的汇编指令，只要输入合法的opcode，解释器就会给出结\
果，是一个虚拟机的简单原型。模拟汇编语言实现一套操作，包含操作码部分的所有操作。

源码：https://github.com/huiliu/Learn/tree/master/VM

参考资料
1.  `《游戏编程模式》 <http://gameprogrammingpatterns.com/bytecode.html>`_
