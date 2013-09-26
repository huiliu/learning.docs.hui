Python-MySQL使用心得
*********************
用\ **Python-MySQLdb**\ 写一个小程序对数据库进行一些更新。输入的数据有下面格式要求：

1.  每一行为一条数据，由五列组成，列与列之间由英文逗号间隔
2.  第一，二，五列为数字，第三，四列为字符型

读入数据后，执行一行相关的SQL语句操作数据库。

如何读入数据
==============
最简单的方法是直接读入每行数据，然后分割为列表。代码如下：

::
    def GetData(FileName):
        lines = open(FileName, 'r').readlines()
        data = [x.split(',') for x in lines]
        return data

另外一种方法是利用生成器（\ **generator**\ ）\ [#]_\ 更加高效完成这个任务。代码如下：

::
    def GetData(FileName):
        lines = open(FileName, 'r').readlines()
        for line in lines:
            yield line


如何连接数据库
================

字符编码的问题
----------------


如何构造SQL语句
==================


如何防止SQL注入
----------------


如何提供用户接口
==================

命令方式
--------


配置文件的形式
------------------


总结
=====


参考资料
==========
.. [#]  http://docs.python.org/2/tutorial/classes.html#generators
.. [#]  https://wiki.python.org/moin/Generators
.. [#]  http://docs.python.org/2.7/library/configparser.html
.. [#]  http://stackoverflow.com/questions/9154998/python-encoding-mysql
.. [#]  http://stackoverflow.com/questions/8365660/python-mysql-unicode-and-encoding
.. [#]  http://www.harelmalka.com/?p=81
.. [#]  http://hiei.yeax.com/archives_165.html
.. [#]  http://www.jb51.net/article/26543.htm
