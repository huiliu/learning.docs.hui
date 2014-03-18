Learning SQL
*************
学习SQL的一些基本语法。\ [#]_

一些基本术语：\ **实体**\ ，\ **列**\ ，\ **行**\ ，\ **表**\ ，\ **结果集**\
，\ **主键**\ ，\ **外键**\ ，\ **事务**\ ，\ **索引**\ ，\ **视图**\ ，\ **约
束**\ 。

SQL语言可以分为几个模块：

1.  **SQL定义语句(Data Definition Language)**\ ，用于定义存储于数据库中的数据结\
    构；

    *   ``CREATE``
    *   ``DROP``
    *   ``ALTER``

2.  **SQL操作数据语句(Data Manipulation Language)**\ ，用于操作数据库中的数据；

    *   ``SELECT``
    *   ``INSERT``
    *   ``UPDATE``
    *   ``DELETE``

3.  **SQL事务语句(Data Control Language)**\ ，用于开始、结束或回滚事务。
    
    *   ``COMMIT``
    *   ``ROLLBACK``
    *   ``GRANT``
    *   ``REVOKE``

一定要注意，不同的数据库软件使用的SQL语法略有不同，如果某条语句提示语法错误请\
查找相应手册，确认语法符合要求。

数据操作
=========

CREATE TABLE
-------------
标准语法定义：

.. sourcecode:: sql

    CREATE [TEMPORARY] TABLE [IF NOT EXISTS] tbl_name
    [(create_definition,...)] [table_options] [select_statement]
    -- 或:
    CREATE [TEMPORARY] TABLE [IF NOT EXISTS] tbl_name [(] LIKE old_tbl_name [)];

*   使用\ ``IF NOT EXISTS``\ 判断表是否存在：

    .. sourcecode:: sql
    
        CREATE TABLE IF NOT EXISTS tbl_name ......

ALTER TABLE
-------------
SQL格式：

.. sourcecode:: sql

    ALTER TABLE tbl_name ADD (col_name1, col_name2);
    ALTER TABLE tbl_name DROP COLUMN col_name;

*   调整列的数据类型（在MariaDB 5.5.32上测试）\ [#]_
    
    .. sourcecode:: sql

        ALTER TABLE tbl_name MODIFY [COLUMN] col_name col_definition

*   创建一个主键（在MariaDB 5.5.32上测试）
    
    .. sourcecode:: sql

        ALTER TABLE tbl_name ADD PRIMARY KEY (index_col_name)

*   删除列

    .. sourcecode:: sql

        ALTER TABLE tbl_name DROP COLUMN col_name;


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
* 两个“\ ``NULL``\ ”不能判断为相等。如果利用“\ ``=``\ ”来判断两个“\ ``NULL``\ ”\
  值，将不会得到正确的结果，而且服务器也不会发出出错提示，这是相当危险的。
* 判断两个“\ ``NULL`` ”相等，需要用到操作符“\ ``IS``\ ”。如下：

.. sourcecode:: sql

    /* 假定superior_emp_id存在NULL值 */
    /* 错误的做法 */
    SELECT emp_id, name FROM employee WHERE superior_emp_id = NULL
    /* 正确的做法 */
    SELECT emp_id, name FROM employee WHERE superior_emp_id IS NULL

另外进行条件筛选时也要考虑全面，注意\ ``NULL``\ 值的存在

分组与聚集
==========
这一部分貌似有点像简单的数据分析和挖掘，当然是最最初级的。
它主要包括：\ **分组**\ 和\ **聚集**\ 两种操作。

* **分组**\ 即针对某一特征的不同值进行分组，分块。
* **聚集**\ 其实是对分组后，每组中的数据进行统计分析。SQL只提供了一些简单的统计
  函数。如\ MAX_\ ，\ MIN_ \ ，SUM_ \ ，COUNT_\ ，AVG_\ 等。

.. _MAX:

.. _MIN:

.. _SUM:

.. _COUNT:

.. _AVG:

``GROUP BY``
-------------
SQL：

.. sourcecode:: sql

    SELECT * FROM tbl_name WHERE conditions GROUP BY col_names;

**SQL的执行顺序：**\ （不同软件实现可能不同）

|   ``FROM`` -> ``WHERE`` -> ``GROUP BY`` -> ``SELECT``

关于\ ``GROUP BY``\ 需要注意的：\ *``SELECT``\ 字句中只能包含常数、聚合函数、\
``GROUP BY``\ 子句中的列（聚合键）*

思考：

.. sourcecode:: sql

    -- 执行一下下面的语句，看看结果如何？分析一下？
    SELECT col_name AS COL_NAME, COUNT(*) FROM tbl_name GROUP BY COL_NAME;

``HAVING``
-----------
``HAVING``\ 子句类似于\ ``WHERE``\ ，为指定条件的语句。但是\ ``HAVING``\ 的条件\
主要是针对聚合结果进行判断。所以，\ ``HAVING``\ 子句的条件可以是什么？不能是什\
么？

SQL格式：

.. sourcecode:: sql

    SELECT * FROM tbl_name WHERE conditions GROUP BY col_names HAVING conditions;
    
思考：

*   包含\ ``HAVING``\ 子句的SQL执行顺序是什么样的？
*   \ **什么时候用\ ``WHERE``\ ？什么时候用\ ``HAVING``\ 呢？**

子查询
======
子查询是指包含在另一个SQL语句内部的查询。它总是被括号包围，且通常在包含语句前执\
行。可以分为两类：

* **非关联子查询**\ 。子查询单独执行，在包含语句之前完成执行，不引用包含语句。
* **关联子查询**\ 。不是在包含语句执行前一次执行完毕，而是为每个候选行执行一次。

非关联子查询
------------
* 如果子查询返回值为单行单列（即，仅为一个值），可以直接用于运算操作。如：


* 如果子查询返回值为多行单列（即，为一个集合），不能用于相等操作，不过可以使用\
  其它用于集合的操作符。如：\ ``IN, NOT IN, ALL, ANY``\ 。也可以使用聚集函数进\
  行统计。

.. sourcecode:: sql

    SELECT COUNT(*) FROM (
                            SELECT 1 FROM City
                            GROUP BY district
                         ) AS d;


* 如果子查询返回（多行）多列。


关联子查询
-----------


多表查询与连接
==============


集合
=====
等同于数学中关于\ `集合`_\ 的一些操作，如并集，交集，差集。
并集操作

.. _集合: http://zh.wikipedia.org/wiki/%E9%9B%86%E5%90%88_(%E6%95%B0%E5%AD%A6)

并集操作\ ``UNION``\ ，\ ``UNION ALL``
----------------------------------------
* ``UNION``\ 连接多个数据后会进行除重、排序。（所以速度后相对慢一点）
* ``UNION ALL``\ 仅仅将两个数据集并在一起，无其它操作，相对于\ ``UNION``\ 快一些

一个简单的例子：

.. sourcecode:: sql

    SELECT 1 NUM, 'abc' STR
    UNION
    SELECT 9 NUM, 'xyz' STR

交集操作\ ``INTERSECT``\ 和\ ``INTERSECT ALL``
-----------------------------------------------
语法与\ ``UNION``\ 一样。在MySQL中没有实现，SQL Server、Oracle和IBM DB2中有实现。

差集操作\ ``EXCEPT``\ 和\ ``EXCEPT ALL``
-----------------------------------------
MySQL中没有实现。

集合操作的一些规则
-------------------
想想数学中的集合操作规则。

指定排序
~~~~~~~~
如果使用\ ``ORDER BY``\ 指定按某一列进行排序，此列名只能是第一个查询的列名。

如：

.. sourcecode:: sql

    SELECT num, name FROM employee 
    UNION
    SELECT product_id, open_b_id FROM account
    ORDER BY num

操作优先级
~~~~~~~~~~
* ``INTERSECT``\ 优先于其它两个操作
* 按从左到右的先后顺序进行操作


条件逻辑
========




事务
====
锁的策略
---------
锁是数据库用于控制数据被并行使用的一种机制。当数据库的一些内容被锁定时，任何对\
这个数据的修改（甚至是读取）都必须等待锁被释放。主流有两种锁策略：

* 数据库的写操作必须申请并获得写锁才能修改数据，而读操作必须申请和获得读锁才能\
  查询数据，多用户可以同时读取数据（即读锁可以分配多个）。一个表（或页或行）只\
  能分配一次分配一个写锁，并且拒绝读请求直至写锁释放。
  **读写是相互排斥的，读写都必须申请锁。**\ SQL Server才用的是此策略，MySQL取决\
  于选择的存储引擎。
* 数据库的写操作必须申请并获得锁才能修改数据，而读操作不需要锁。但是数据库服务\
  器必须保证读操作从开始到结束看到的是一个一致的数据视图。此方法称为\ **版本控\
  制**\ 。\ **写操作需要锁，读不需要锁，但服务器必须保证读时数据一致。**\ Orale\
  采取的是此策略。

锁的粒度
--------
* **表锁**  阻止多用户同时操作一个表的数据
* **页锁**  阻止多用户同时操作表中同一页（2-16KB内存空间）的数据
* **行锁**  阻止多用户同时操作表中同一行的数据

SQL Server使用表锁，页锁和行锁，Oracle只有行锁，MySQL取决于存储引擎的选择。在某\
些情况下，SQL Server会逐步升级锁，Oracle从不升级锁


索引与约束
==========

视图
====

参考资料
==========
.. [#]  Mick, 孙淼，罗勇译；《SQL基础教程》；人民邮电出版社
.. [#]  http://dev.mysql.com/doc/refman/5.5/en/alter-table.html
