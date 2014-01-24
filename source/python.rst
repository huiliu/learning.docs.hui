Learning Python
*******************

Calling an external command in Python
======================================
在\ `stackoverflow`_\ 看到一个话题讨论在Python中如何调用外部程序的。学习了一下\
，原来新的改进方法在模块\ ``subprocess``\ 中，在\ `PEP 324`_\ 中有详细介绍。

调用外部程序的常用方法有：
* 函数"``os.popen``" 被"``subprocess.Popen``"取代
* 函数"``os.system``" 被"``subprocess.call``"取代

另外Python\ `文档`_\ 中亦有详细介绍。

.. _stackoverflow: http://stackoverflow.com/questions/89228/calling-an-external-command-in-python
.. _PEP 324: http://www.python.org/dev/peps/pep-0324/
.. _文档: http://docs.python.org/2/library/subprocess.html#replacing-older-functions-with-the-subprocess-module


The Python yield keyword explained
======================================
参考stackoverflow上的\ `The Python yield keyword explained`_\ 。

.. _The Python yield keyword explained: http://stackoverflow.com/questions/231767/the-python-yield-keyword-explained


三元运算
==========
类似于C和其它语言中的三元运算\ ``(condition) ? a : b``\ 。Python中也提供了一个类似的三元运算：\ [#]_ ::

    # 如果y > 0, x = a；反之x = b
    x = a if y > 0 else b

首先会对中间条件求值，如果为True，再对\ ``if``\ 左边表达式求值，否则对\ ``else``\ 后面的表达式求值。


嵌套函数的调试
================
在写scrapy爬虫时用于嵌套函数，但怎么都不输出期望的结果，再结果，用\ ``pdb``\ 来\
进行调试，调试进程却怎么也进入不了嵌套函数内部。代码大概如下：\ ::

    def parse(self, response):
        
        def News(hxs, xpath, pri):
            for tmp in hxs.xpath(xpath):
                item[..] = ..

                request = Request(url, callback=...)
                request.meta['item'] = item

                yield request

        hxs = Selector()
        xpath1 = '...'
        News(hxs, xpath1, 1)

``pdb``\ 调试进程进行到\ ``def News(hxs, xpath, pri)``\ 就退出了，提示：“\
**Generator exit**\ ”之类的什么。

浮点保留有效数字
=================
内置函数\ ``round``\ 可以完成此任务。

脚本中获取其所在路径
=====================
为了保证路径的正确，在脚本中需要获取其所在的路径，有多种不同的方法可以得到此值\
，不过它们确有不同的适用范围：

.. sourcecode:: python

    import sys
    import os

    # 使用py2exe，无法使用变量__file__，而且sys.argv[0]取得的值也可能不正确
    # 只有sys.executable可以取得正确的期望值
    print(os.path.dirname(os.path.realpath(__file__)))
    print(sys.executable)
    print(sys.argv[0])
=======
函数参数部分求值
=================
**currying**\ [#book]_ ::

    def foo(x, y, z):
        return x + y + z

    from functools import partial
    f = partial(foo, 1, 2)          # 为foo的参数x, y提供值
    f(3)                            # 调用foo(1, 2, 3), 结果为6

enumerate函数和zip函数
========================
请看下面的代码：\ [#book]_ \ ::

    # Part 1
    i = 0 
    for x in s:
        # ......
        i += 1

    # 替代方法
    for i, x in enumerate(s):
        # ......

    # Part 2
    i = 0
    while i < len(s) and i < len(t)
        x = s[i]
        y = t[i]
        # ......
        i += 1

    # 替代方法
    for x, y in zip(s, t):
        # ......

``enumerate``\ 创建一个迭代器，返回一个元组序列(0, s[0]), (1, s[1]) ...
``zip``\ 包装的两个序列如果长度不等，较短的索引完将结束。在Python 2中，\
``zip``\ 将一次性用两个序列生成一个元组列表，数据量较大时可能出现不可预测的结\
果，函数\ ``itertools.izip()``\ 的实现效果与\ ``zip``\ 一致，不过每次仅生成一\
个元组，Python 3中，\ ``zip``\ 生成值的方式与之一样。


参考资料
==========
.. [#]  http://blog.csdn.net/jiangnanandi/article/details/3322192
.. [book]  David M. Beazley Python Essential Reference (4th)
