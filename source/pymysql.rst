Python-MySQL使用心得
*********************
用\ **Python-MySQLdb**\ 写一个小程序对数据库进行一些更新。输入的数据有下面格式\
要求：

1.  每一行为一条数据，由五列组成，列与列之间由英文逗号间隔
2.  第一，二，五列为数字，第三，四列为字符型

读入数据后，执行一行相关的SQL语句操作数据库。

如何读入数据
==============
最简单的方法是直接读入每行数据，然后分割为列表。代码如下：\ ::

    def GetData(FileName):
        lines = open(FileName, 'r').readlines()
        data = [x.split(',') for x in lines]
        return data

另外一种方法是利用生成器（\ **generator**\ ）\ [#]_\ 更加高效完成这个任务。代码\
如下：\ ::

    def GetData(FileName):
        lines = open(FileName, 'r').readlines()
        for line in lines:
            yield line


如何连接数据库
================
按照Python数据库API规范（PEP 249）规定\ ``MySQLdb``\ 包含一个标准的方法\
``connect``\ 用来连接数据库。如：\ ::

    import MySQLdb

    conn = MySQLdb.connect(host='localhost', user='root', passwd='', db='test')


字符编码的问题
----------------
``MySQLdb``\ 建立与数据库的连接时可以通过额外的参数来指定字符集：\ ::

    import MySQLdb
    conn = MySQLdb.connect(......, charset='utf8', useunicode=True)


如何构造SQL语句
==================


如何防止SQL注入
----------------
``MySQLdb.escape_string()``\ 可以将用户的输入进行转义，防止SQL注入。

如果字段中有中文，\ ``MySQLdb.escape_string()``\ 可能会出错：

.. sourcecode:: python

    Traceback (most recent call last):
      File "parse.py", line 218, in <module>
        insert(path_name, tbl_name)
      File "parse.py", line 171, in insert
        value += "'%s'," % MySQLdb.escape_string(v)
    UnicodeEncodeError: 'ascii' codec can't encode characters in position 0-2: ordinal not in range(128)

从上面的提示可以看到，是因为变量v使用的是\ `ascii`\ 编码，由此联想到python默认\
使用的是\ `ascii`\ ，只需要将其调整一下应该就可以了：

.. sourcecode:: python

    # 将其插入到上面代码的前面再运行，就不会出错了
    import sys
    reload(sys)
    sys.setdefaultencoding('utf-8')

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
