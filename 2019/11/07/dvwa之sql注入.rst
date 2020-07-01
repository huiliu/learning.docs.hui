DVWA之SQL注入
**************



.. author:: default
.. categories:: dvwa, SQL注入
.. tags:: dvwa, SQL注入
.. comments::

试着用SQL注入的方式突破DVWA中的\ **Brute Force**\ 部分，发现一些问题：

第一次尝试
===========
使用\ ``1' or 1=1;--``\ 进行注入，失败！
但是得到了一个错误提示：

.. code-block:: none

    You have an error in your SQL syntax; check the manual that corresponds to your MariaDB server version for the right syntax to use near '--' AND password = '8d777626bb90b3fe578edd23f606e9a9'' at line 1

有点懵，应该是输入导致SQL语句执行错误。从中得到了一些信息：数据库是\ **MariaDB server**\ 。
SQL的注释符不就是 *\-\-* 吗，怎么会报错呢？查一下\ *MariaDB*\ 的\ `网站 <https://mariadb.com/kb/en/library/comment-syntax/>`_\ 看看。
原来\ **注释符 *\-\-* 后面是有一个空格的**\ 。

重新使用\ ``1' or 1=1;-- ``\ 注入，成功！哈哈

SQL注释
--------
*   MariaDB

    *   "**#**"到行尾
    *   "**-- **"(注意空格)到行尾
    *   C风格的多行注释"**/\***"与"***/"间的内容

*   MySQL
*   MSSQL Server


总结
======
*   SQL语法不熟悉