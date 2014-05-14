MySQL服务器状态
*****************


线程和连接的统计信息
====================
应该关注于服务器一段时间的变化，变化率，而不是总的计数

*   ``connections, max_used_connections, threads_connected``
*   ``aborted_clients, aborted_connections``
*   ``bytes_received, bytes_sent``
*   ``slow_launch_threads, threads_cached, threads_created, threads_running``

二进制日志
==========

``binlog_cache_use, binlog_cache_disk_use``


命令计数器
==========
``Com_*``\ 变量记录了执行过的SQL和C API命令数。如\ ``Com_select``\ 记录的是
*SELECT*\ 语句的数目；\ ``Com_change_db``\ 记录的是使用\ *USE*\ 或C API改变默认
数据库的次数。


临时文件和表
============
.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'created_tmp%';

Handler操作
===========
Handler API是MySQL与存储引擎之间的接口。观察\ ``Handler_*``\ 变量能让你了解服务
器做的最多的是哪些工作。

.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'Handler%';

MyISAM键缓冲
============
.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'KEY%';


查询缓存
========
.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'Qcache_%';

文件描述符
==========
.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'open_%';


各种类型的SELECT
================
.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'select%';

排序
=====

.. sourcecode:: sql

    SHOW GLOBAL STATUS LIKE 'sort%';


表锁定
=======
状态变量\ ``table_locks_immediate``\ 和\ ``table_locks_waited``


InnoDB STATUS
==============

.. sourcecode:: sql

    SHOW ENGINE INNODB STATUS;

其它
=====

进程列表
--------
.. sourcecode:: sql

    SHOW PROCESSLIST;

    SHOW ALL PROCESSLIST;

互斥量
-------
.. sourcecode:: sql

    -- 貌似5.5不太一样
    SHOW MUTEX STATUS;

复制状态
=========
.. sourcecode:: sql

   SHOW MASTER STATUS;

   SHOW BINARY LOGS;

状态变量\ ``master_log_file, master_log_pos``\ 和\ ``relay_log_file,
relay_log_pos, relay_master_log_file, relay_master_log_pos``


参考资料
========
1.  高性能MySQL     P423
