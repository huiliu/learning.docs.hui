MySQL权限管理
**************
当用户访问MySQL Server时，Server的存取控制分为两步：

存取控制步骤
============
1.  身份核实。根据访问的源主机，用户名，密码来判断是否有连接权限
2.  请求核实。对通过身份核实的用户所发出的请求，对比\ ``mysql``\ 库中的\
    ``user, host, db, tables_priv``\ 等权限控制表来核实用户是否有权限执行数额请\
    求操作。

阶段一：身份核实
----------------
进行连接身份核实时，MySQL Server会将客户端的输入信息与\ ``mysql.user``\ 表中的\
``user, host, password``\ 字段进行核实，以判断是否允许连接。（注意MySQL Server\
没有直接排除用户，主机的功能）

在\ ``mysql.user``\ 中经常会出现多个匹配项目，MySQL Server的原则是：\ **将第一个
与\ ``user``\ 匹配的权限返回**\ 。（注意不是路由的最大匹配原则）当MySQL Server载
入授权表时会以\ ``host, user``\ 字段对授权表进行排序，匹配时会按照排序后的先后顺
序来匹配。排序的基本原则是：\ **指明越具体排名越靠前**\ 。例如\ ``mysql.user``\
中数据如下：

+-----------+----------+
| Host      | User     |
+-----------+----------+
| %         | root     |
+-----------+----------+
| %         | jeffrey  |
+-----------+----------+
| localhost | root     |
+-----------+----------+
| localhost |          |
+-----------+----------+

排名后应该为：

+-----------+----------+
| Host      | User     |
+-----------+----------+
| localhost | root     |
+-----------+----------+
| localhost |          |
+-----------+----------+
| %         | jeffrey  |
+-----------+----------+
| %         | root     |
+-----------+----------+

那么当\ ``jeffrey``\ 从\ ``localhost``\ 来访问时，实际上会匹配第二条（localhost\
, 用户名为匿名）。

由于多条权限匹配时不确定当前用户时可以使用\ ``SELECT CURRENT_USER()``\ 来查看当\
前用户。

阶段二：访问控制，请求核实
--------------------------
对于用户的每个请求MySQL Server对核查授权表以检查是否有相应的权限。授权表主要包\
括：\ ``mysql.(user, host, db, tables_priv)``\ 。需要说明的是\ ``mysql.user``\
中的权限设定是全局的，应该只在此表中授予超级用户权限。

权限更改何时生效
----------------
1.  当mysqld启动时，所有授权表的内容被读进内存并且从此时生效。
2.  当服务器注意到授权表被改变了时，现存的客户端连接有如下影响：
        1.  表和列权限在客户端的下一次请求时生效。
        2.  数据库权限改变在下一个USE db_name命令生效。
        3.  全局权限的改变和密码改变在下一次客户端连接时生效。
3.  如果用\ ``GRANT、REVOKE``\ 或\ ``SET PASSWORD``\ 对授权表进行修改，服务器会\
    注意到并立即重新将授权表载入内存。
4.  如果你手动地修改授权表(使用\ ``INSERT、UPDATE或DELETE``\ 等等)，你应该执行\
    ``mysqladmin flush-privileges或mysqladmin reload``\ 告诉服务器再装载授权表\
    ，否则你的更改将不会生效，除非你重启服务器

用户管理
========
增加新用户
----------
一般可以通过\ ``GRANT, INSERT INTO``\ 语句来建立一个新账户并授予相应的权限。如：

.. sourcecode:: sql

    GRANT ALL PRIVILEGES ON test.* TO 'user'@'host' IDENTIFIED BY 'password' [WITH OPTIONS];
    INSERT INTO mysql.user (Host, User, Password) VALUES ('host', 'user', '');
    FLUSH PRIVILEGES;

更改密码
--------
正常情况下，使用拥有相应权限的用户登陆MySQL Server，执行下面的操作：

.. sourcecode:: sql

    -- 方法一
    UPDATE mysql.user SET Password=PASSWORD('newpassword') WHERE User='user';
    -- 方法二
    SET PASSWORD FOR 'user'@'host' = PASSWORD('newpassword')

重置root密码
^^^^^^^^^^^^
当忘记root密码时，需要关闭MySQL Server，

1.  方法一：然后增加选项\ ``--skip-grant-tables``\ 来重启Server，再以root登陆\
    MySQL Server，执行上面的语句重置密码。
2.  方法二：另外还可以将上面的\ ``SET ...``\ 语句的内容写入到一个文件中（如\
    mysql.init），然后用\ ``mysqld_safe --init-file=./mysql.init &``\ 来启动\
    Server。（注意删除mysql.init，此方法不如一安全）

限制账户资源
------------
在MySQL Server配置选项中有选项\ ``max_user_connections``\ 可以全局控制单个用户\
的同时连接数。

在授权表\ ``mysql.user``\ 中有几个额外的选项可以更加精确的用户访问资源进行限制：

*   ``max_questions``
*   ``max_updates``
*   ``max_connections``
*   ``max_user_connections``

可以在对用户授权时指定：

.. sourcecode:: sql

    GRANT ALL ON customer.* TO 'francis'@'localhost' IDENTIFIED BY 'frank'
        WITH MAX_QUERIES_PER_HOUR 20
            MAX_UPDATES_PER_HOUR 10
            MAX_CONNECTIONS_PER_HOUR 5
            MAX_USER_CONNECTIONS 2;

修改用户账户限制：

.. sourcecode:: sql

     GRANT USAGE ON *.* TO 'francis'@'localhost' WITH MAX_QUERIES_PER_HOUR 100;

该语句没有改变账户的已有权限，只修改了指定的限制值。

**取消限制**\ 只需要将相应的值设定为0即可。\ **重置所有帐户**\ 的记数：\
``FLUSH USER_RESOURCES``\ 或者\ ``FLUSH PRIVILEGES``\ 。

参考资料
========
1.  http://doc.mysql.cn/mysql5/refman-5.1-zh.html-chapter/
