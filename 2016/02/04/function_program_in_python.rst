function program in python
**************************

Python中的函数式编程。

.. author:: default
.. categories:: program
.. tags:: python, program, function program
.. comments::
.. more::




函数式编程
============
高阶函数(hight order function)\ [1]_\ 是一类特殊的函数，它至少满足下面两个条件
中的一个：

    1.  接受一个或多个函数作为输入参数
    2.  返回值为函数

函数式编程(functional programming)\ [2]_ [3]_ [4]_\ 是一种编程范式。


``filter``
===========
假定你有一个整数列表，你想取出其中的偶数项。

1.  可以使用\ ``for循环`` 来完成：

    .. sourcecode:: python

        integers = range(0, 10)

        even = []
        for i in integers:
            if i % 2 == 0:
                    even.append(i)

        >>> even
        >>> [0, 2, 4, 6, 8]

2.  使用\ ``filter``\ 函数加\ ``高阶函数``\ 可以更简洁的完成：

    .. sourcecode:: python

        even = filter(lambda x: x % 2 == 0, integers)

    上面只用一行代码就可以了，而\ ``for循环``\ 的方法需要4行代码。在这里使用了
    匿名函数\ ``lambda`` [5]_\ 。如果用普通函数将\ ``lambda``\ 函数展开，则：

    .. sourcecode:: python

        def is_event(x):
            return x % 2 == 0

        even = filter(is_event, integers)

3.  也可以用\ ``列表解析(list comprehension)``\ 的方法:

    .. sourcecode:: python

        even = [x for x in integers if x % 2 == 0]

    列表解析的方法更加简洁，没有任何函数调用。

比较上面三种方案，\ ``for循环``\ 代码量最大，但便于新手理解；\ ``filter``\ 和
``列表解析``\ 都非常简洁，但\ ``列表解析``\ 的方法更加清晰，语义与自然语言相近
，速度更快（函数调用都没有）。

``map``
========
``map``\ 函数将函数作用于列表中的每个元素，并将结果返回形成一个新列表。如：

.. sourcecode:: python

    map(lambda x: x*x, integers)
    >>> [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

同样，可以使用\ ``列表解析``\ 来达成此任务。

.. sourcecode:: python

    square_list = [x * x for x in integers]


``reduce``
============
Python 3中将\ ``reduce``\ 函数移到了\ :py:func:`reduce`\ 。

.. py:class:: functools

    .. py:function:: functools.reduce(function, iterable[, initializer])

        return a single value.

        :param callable function: 需要两个输入参数的函数
        :param iterable iterable: 一个可迭代对象
        :param initializer: 指示iterable的起始位置
        :return: a single value

``reduce``\ 函数等同于\ ::

    def reduce(function, iterable, initializer=None):
        it = iter(iterable)
        if initializer is None:
            value = next(it)
        else:
            value = initializer
        for element in it:
            value = function(value, element)
        return value

如： ``reduce(lambda x, y: x+y, [1, 2, 3, 4, 5])``\ 等同于\
``((((1+2)+3)+4)+5)``

本文部分参考\ `Beyond the For-Loop: Higher Order Functions and List
Comprehensions in Python
<http://erokar.svbtle.com/beyond-the-forloop-higher-order-functions-in-python>`_

.. todo::

   添加其它函数式编程特性。

参考资料
=========

..  [1]    `Higher-order function
            <https://en.wikipedia.org/wiki/Higher-order_function>`_

..  [2]    `Functional programming
            <https://en.wikipedia.org/wiki/Functional_programming>`_

..  [3]    `函数式编程 <http://coolshell.cn/articles/10822.html>`_

..  [4]    `函数式编程初探
            <http://www.ruanyifeng.com/blog/2012/04/functional_programming.html>`_

..  [5]     `Using lambda Functions
    <http://www.diveintopython.net/power_of_introspection/lambda_functions.html>`_
