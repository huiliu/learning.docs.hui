MySQL日志相关服务器变量
***********************

MySQL Server参数中与日志相关的选项。如：\ ``Error Log``, ``General query
log``, ``Binary log``, ``Relay log``, ``Slow query log``\ 。

.. author:: default
.. categories:: database, mysql
.. tags:: database, mysql, dba
.. comments::
.. more::

MySQL Server可以生成的日志分为：

1.  ``Error log``\ 用于保存服务器的启动，停止，运行及出错信息；
2.  ``General query log``\ 记录客户连接及请求的SQL语句，主要用于调试客户端与服\
    务商的连接；
3.  ``Binary log``\ 记录变更数据的语句（依binlog格式不同有所差异），用于主从复\
    制，备份及恢复；
4.  ``Relay log``\ 产生于slave服务器，记录着从主数据库接收到的数据变更；
5.  ``Slow query log``\ 请求执行时间大于\ ``long_query_time``\ 设定时长。

主意：本文中使用的MySQL Server服务器变量名均为通过\ ``show variables var_name``\
查看。在全局配置文件\ ``/etc/my.cnf``\ 中使用的变量名可能与此不同。主要差别是变\
量名中的连接符，配置文件中使用的可能是中划线（-），而利用SQL查看的变量名中的连\
接符均为下划线（_）

Error Log
==========
与\ ``Error log``\ 相关的配置是：\ ``log_error = file_name``\ 。其中：\
*file_name*\ 默认为\ *数据目录下的mysqld.err*\ 。另外还有服务器变量\
``log_warning = 0|1``\ 来控制日志的输出级别。


General query log
==================
变量\ ``general_log = OFF|ON``\ 是控制开关，默认为\ ``OFF``\ ；另外变量\
``general_log_file = file_name``\ 用于指定保存日志的文件名。默认为：\
*hostname.log*

.. warning::

    General log按照收到客户端请求SQL的顺序记录在日志文件中，其顺序可能与服务器
    的执行顺序不一致。

Binlog
======
相关服务器变量：

1.  ``log_bin = OFF|ON``\ 控制着是否开启binlog功能。binlog的默认保存文件名为：\
    *hostname-bin.xxxxxx*
2.  ``sql_log_bin``\ 控制着当前会话是否开启binlog功能。

需要注意：MySQL Server版本不同其binlog格式可能不相同。

过滤器
------
binlog还可以设置过滤器，用于指定记录某些数据库的操作，忽略哪些等：（注意下面的\
系统境变量都只能对应一个值，对于有多个值，可以重复）

1.  ``binlog_do_db = dbname``   记录数据库\ *dbname*\ 的binlog。
2.  ``binlog_ignore_db = dbname``
3.  ``replication_db_db = dbname``\ 在slave中设置。表示复制某个数据库。
4.  ``replication_ignore_db = dbname``

.. todo::
    
    需要了解四个过滤器的作用域，生效顺序。
    《高可用的MySQL》中有详细介绍binlog过滤器的工作原理，生效顺序。

    我的想法是不要混合使用，根据需求最小设置。

另外，在slave上如果开启binlog功能，默认slave是不会将来自master的操作记录到自己的
binglog中，如果需要记录来自master的binlog，需要将\ ``log_slave_updates``\ 设置为
：\ ``ON``\ 。如果想建立一个层级master-slave结构，在slave上必须开启此项。


binlog的清除
------------
由于binlog会占用大量的磁盘空间，所以不得不清除binlog。清除方法有：

1.  ``expire_logs_days = N``\ binlog最长保存周期为N天，超过此天数将被Server自动\
    删除。个人认为对于稳定的服务（产生的数据量稳定），且建立的主从复制，此功能\
    是一个不错的选择。
2.  ``RESET MASTER``
3.  ``PURGE BINARY LOGS``\ PURGE语句可以指定log名或者日期。相应的语法：

    .. sourcecode:: text

        PURGE { BINARY | MASTER } LOGS { TO 'log_name' | BEFORE 'datetime' }

4.  ``mysqladmin flush-logs``

由于binlog会被用于主从复制，所以删除binlog时一定要小心，如果删除了没有同步到从\
库的binlog，就悲剧了。安全的方法是使用SQL语句\ ``PURGE BINARY LOGS``\ 执行些语\
句时，MySQL Server会主动检查binlog使用情况。

所以可以通过\ ``PURGE BINARY LOGS``\ 建立一个binlog的备份策略：

1.  根据实际情况设定一个合理binlog文件滚动值\ ``max_bilog_size = N``\ 当binlog\
    文件大小超过其设定时。默认大小为1G（最大值），最小值为4KB。服务器会打开一个\
    新的文件写入binlog。事务不会被分割到不同的binlog文件中。

2.  定期对旧的binlog文件进行物理备份
3.  运行\ ``PURGE``\ 语句清除binlog


磁盘缓存问题
------------
默认情况下，binlog并不会马上同步写到磁盘上，所以如果在写至磁盘前机器或操作系统崩
溃，就会造成部分binlog丢失。服务器变量\ ``sync_binlog = N``\ 可以减小这种损失，
它将在执行N次写操作之后，强制同步binlog至磁盘。可以想像将其值写为\ ``1``\ ，应该
是最为安全的，不过服务器也会变慢（WHY?）。

另外，对于支持事务的\ ``innoDB``\ 引擎，执行一个事务提交\ ``COMMIT``\ 后，MySQL
Server会先将事务写入到binlog，然后再提交给innoDB引擎。如果在再次操作中间出现故障
，MySQL Server重启时会对事务进行回滚，而binlog中的数据将依旧存在，此时会引起数据
不一致的情况发生。开启选项\ ``innodb_support_xa = ON``\ 可以解决此类问题，该选项
用于保证binlog文件与innoDB数据文件同步。

缓存调优问题
------------
与binlog相关的缓存的变量有：

1.  ``binlog_cache_size``   用于缓存在执行事务时需要写入到binlog的变更的空间大\
    小。MySQL Server会为每个客户分配缓存空间。
2.  ``binlog_stmt_cache_size``  statement cache.
3.  ``max_bilog_cache_size``    如果一个事务需要的内存大于此值，MySQL Server会\
    报错
4.  ``max_bilog_stmt_cache_size``   statement cache

MySQL Server还有对应的四个状态值用于记录binlog缓存的使用情况：

1.  ``Binlog_cache_disk_use``\ 使用磁盘缓存的事务次数。如果此值较大则应该增加\
    binlog的缓存大小：\ ``binlog_cache_size``
2.  ``Binlog_stmt_cache_disk_use``  对比上面想一下

binlog的格式
------------
binlog中有三个方式来保存数据的变化：

1.  基于语句的（statement-based）
2.  基于行的（row-based）
3.  混合的（statement-based）

慢查询日志(Slow query log)
============================
与慢查询相关的重要系统变量有：

1.  ``slow_query_log = OFF|ON`` 慢查询日志是否开启的开关
2.  ``long_query_time = N`` 判断是否属于慢查询的阀值。单位microsecond
3.  ``min_examined_row_limit = 0``  被影响的最小行数
4.  ``log_queries_not_using_indexes = OFF|ON``  是否记录未使用索引的查询。\
    如果一个经常执行的SQL操作没有使用索引，开启此选项，可以导致慢查询日志文件\
    快速增长。MySQL 5.6.5引入了一个新的参数\
    ``log_throttle_queries_not_using_indexes``\ 用于设定每分钟记录（非索引操作\
    ）的最大次数。
5.  ``log_slow_admin_statements = OFF|ON`` 是否记录执行较慢的admin操作
6.  ``slow_query_log_file = file_name`` 慢查询日志文件名

决定一个请求是否被记录至慢查询日志的步骤：

1.  选项\ ``log_slow_admin_statements``\ 开启或查询为非administrative
    statement
2.  执行时间超过\ ``long_query_time``\ 阀值或查询未使用索引且选项\
    ``log_queries_not_using_indexes``\ 开启
3.  操作影响的行数超过\ ``min_examined_row_limit``\ 设定值
4.  满足变量\ ``log_throttle_queries_not_using_indexes``\ (MySQL 5.6.5引入)的限\
    制

分析慢查询日志文件
-------------------
使用命令\ ``mysqldumpslow``\ 来统计分析慢查询日志。

.. warning::

    需要注意慢查询日志的访问安全，因为其中可能会记录包含密码之类的敏感信息

参考资料
========
1.  MySQL Reference Manual 5.2
