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

参考资料
==========
.. [#]  http://blog.csdn.net/jiangnanandi/article/details/3322192
