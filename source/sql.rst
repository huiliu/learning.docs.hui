Learning SQL
*************
学习SQL的一些基本语法。

一些基本术语：\ **实体**\ ，\ **列**\ ，\ **行**\ ，\ **表**\ ，\ **结果集**\
，\ **主键**\ ，\ **外键**\ 。

SQL语言可以分为几个模块：\ **SQL方案语句**\ ，用于定义存储于数据库中的数据结\
构；\ **SQL数据语句**\ ，用于操作数据库中的数据；\ **SQL事务语句**\ ，用于开\
始、结束或回滚事务。

条件过滤-WHERE子句
==================
* 比较操作符有：\ ``=, !=, <>, <, >, LIKE, IN, BETWEEN, IS``\。
* 逻辑操作符有：\ ``AND, NOT, OR``\ 。

范围条件-\ **BETWEEN**
----------------------
即：\ ``a > 0 and a < 10``\ 等同于\ ``a BETWEEN 0 10``\ 。例如下列两条SQL语句等\
同。

.. sourcecode:: sql

    SELECT name FROM employee WHERE start_time < '2007-01-01' AND start_time < '2005-01-01'

    SELECT name FROM employee WHERE start_time BETWEEN '2005-01-01' '2007-01-01'

成员条件-\ **IN**
-----------------
当对某个值的限制为一个有限集合时，SQL可以写为：

.. sourcecode:: sql

    SELECT name FROM employee WHERE district = 'CHN' OR district = 'CHK' OR district = 'CD' OR district = 'SAV'

使用\ **IN**\ 可以写的更为简洁，如：

.. sourcecode:: sql

    SELECT name FROM employee WHERE district IN ('CHN', 'CHK', 'CD', 'SAV')

除了自定义集合，也可以使用子查询产生的集合。如：

.. sourcecode:: sql

    SELECT name FROM employee WHERE district IN (SELECT district_code IN district WHERE people_count < 1000000)

对于\ **IN**\ 也可以使用“非”操作，即表示不在集合内。关键字为：\ **NOT IN**\ 。

.. sourcecode:: sql

    SELECT name FROM employee WHERE district NOT IN ('CHN', 'CHK', 'CD', 'SAV')

匹配条件-\ **LIKE**\， 通配符，正则表达式
------------------------------------------
在各种匹配条件中，也可以使用内置函数。如：

.. sourcecode:: sql

    SELECT name FROM employee WHERE LEFT(lname, 1) = 'T'

上面使用内置函数\ ``LEFT``\ 提取\ *lname*\ 的首字母，然后与字母“T”进行比较。

* ``LIKE``\ 可以使用通配符和一些简单的正则表达式。如下表：

+--------+--------------------------------------------------------+
| 符号   | 匹配                                                   |
+========+========================================================+
| \%     | 匹配任意数目的字符（包括0）。类似于正则表达式中的“.\*” |
+--------+--------------------------------------------------------+
| \_     | 匹配一个字符。类似于正则表达式中的“.”                  |
+--------+--------------------------------------------------------+
| [abc]  | 匹配集合中的元素                                       |
+--------+--------------------------------------------------------+
| [^abc] | 不匹配集合中的元素                                     |
+--------+--------------------------------------------------------+

* 另外可以使用函数\ ``REGEXP``\ 来使用正则表达式来进行匹配。如：

.. sourcecode:: sql

    SELECT name FROM employee WHERE lname REGEXP '^[FG]'

在Oracle中使用函数\ ``regexp_like``\ 代替\ ``REGEXP``\ ，而在MS SQL SERVER中可\
以直接在\ ``LIKE``\ 中使用正则表达式。

关于\ **NULL**\ 值的操作
------------------------
当使用\ ``NULL``\ 需要注意：

* 表达式的值可以为“\ ``NULL``\ ”，但为不能等于“\ ``NULL``\ ”。
* 两个“\ ``NULL``\ ”不能判断为相等。如果利用“\ ``=``\ ”来判断两个“\ ``NULL`` \”值，将不会得到正确的结果，而且服务器也不会发出出错提示，这是相当危险的。
* 判断两个“\ ``NULL`` ”相等，需要用到操作符“\ ``IS``\ ”。如下：

.. sourcecode:: sql

    /* 假定superior_emp_id存在NULL值 */
    /* 错误的做法 */
    SELECT emp_id, name FROM employee WHERE superior_emp_id = NULL
    /* 正确的做法 */
    SELECT emp_id, name FROM employee WHERE superior_emp_id IS NULL

另外进行条件筛选时也要考虑全面，注意\ ``NULL``\ 值的存在
