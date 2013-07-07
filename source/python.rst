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

参考资料
==========
