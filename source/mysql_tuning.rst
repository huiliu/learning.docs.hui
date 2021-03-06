MySQL通用调优原则
******************
如果服务器使用单一存储引擎，调优相对容易的多。如，只使用MyISAM表，就可以完全禁止
InnoDB；反之类似。

查询缓存
=========
MySQL可以缓存将查询结果，当客户再次查询时会直接将结果返回给用户。当服务端收到一
个请求后，在解析请求之前会先检查\ *查询缓存*\ 是否有相应结果，如果有，则直接将结
果返回给用户，并增加\ ``Qcache_hits``\ 的计数（而不增加\ ``Com_select``\ 计算）
。由于检查缓存时MySQL会使用查询语句，而且要求完全一致，区别大小写。

即使开启查询缓存的功能，MySQL也不会缓存所有的查询。下列情况的查询就不会被缓存：\
[#query_cache]_

*   查询是一个外部查询的子查询
*   被存储函数(stored function)，触发器(trigger)，事务(event)执行的查询
*   查询使用了用户自定义函数或存储函数
*   查询使用了用户变量或本地存储程序的变量
*   数据库\ *mysql, INFORMATION_SCHEMA和performance_schema*\ 中的表
*   任何下面形式的查询

    .. sourcecode:: sql

        SELECT ... LOCK IN SHARE MODE;
        SELECT ... FOR UPDATE;
        SELECT ... INTO OUTFILE ...
        SELECT ... INTO DUMPFILE ...
        SELECT * FROM ... WHERE autoincrement_col IS NULL;

*   另外，如果查询中包含以下特殊函数，查询结果也不会被缓存。

相关服务器变量和状态值
----------------------
与查询缓存相关的MySQL变量有：(不同版本可能有所不同，下面为MariaDB 5.5.36）

+------------------------------+---------+-------------------------------------+
| Variable_name                | 默认值  | 说明                                |
+------------------------------+---------+-------------------------------------+
| query_cache_limit            | 1048576 | 可以缓存的最大查询结果（字节）      |
+------------------------------+---------+-------------------------------------+
| query_cache_min_res_unit     | 4096    | 缓存最小分配空间                    |
+------------------------------+---------+-------------------------------------+
| query_cache_size             | 0       | 缓存大小                            |
+------------------------------+---------+-------------------------------------+
| query_cache_strip_comments   | OFF     |                                     |
+------------------------------+---------+-------------------------------------+
| query_cache_type             | ON      | 缓存类型（如何缓存）                |
+------------------------------+---------+-------------------------------------+
| query_cache_wlock_invalidate | OFF     | 详情请看\ `MySQL Manual`_           |
+------------------------------+---------+-------------------------------------+

与查询缓存相关的状态值有：

+-------------------------+--------------------------------------------+
| Variable_name           | 说明                                       |
+-------------------------+--------------------------------------------+
| Qcache_free_blocks      | 查询缓存中未使用的内存块                   |
+-------------------------+--------------------------------------------+
| Qcache_free_memory      | 查询缓存中未使用的内存                     |
+-------------------------+--------------------------------------------+
| Qcache_hits             | 缓存命中次数                               |
+-------------------------+--------------------------------------------+
| Qcache_inserts          | 插入记录至缓存中的次数                     |
+-------------------------+--------------------------------------------+
| Qcache_lowmem_prunes    | 当内存不足时从查询缓存中删除的记录数       |
+-------------------------+--------------------------------------------+
| Qcache_not_cached       | 没有被缓存的查询次数                       |
+-------------------------+--------------------------------------------+
| Qcache_queries_in_cache | 缓存中的查询数                             |
+-------------------------+--------------------------------------------+
| Qcache_total_blocks     | 查询缓冲的总块数                           |
+-------------------------+--------------------------------------------+


.. _MySQL Manual:
   http://dev.mysql.com/doc/refman/5.5/en/server-system-variables.html#sysvar_query_cache_wlock_invalidate

Tips 关注点
-----------

*   查询命中率

    .. math::

        \% = \frac{Qcache\_hits}{Qcache\_hits + Com_select}


MyISAM的键缓存
==============
MyISAM自身只缓存了索引，没有数据（数据由操作系统缓存）。最重要的选项是\
``key_buffer_size``\ 。默认情况下，MyISAM将所有索引缓存在默认的键缓存中，但是可
以创建多个命名键缓存区。例如在配置文件中添加下面两行，就会创建名为\
*key_buffer_1*\ 和\ *key_buffer_2*\ 的两个键缓存区，大小都为1G。此时MySQL就有三
个键缓存区了。

.. sourcecode:: ini

    key_buffer_1.key_buffer_size = 1G
    key_buffer_2.key_buffer_size = 1G

可以通过\ *CACHE INDEX*\ 将表映射到缓存。如，下面将表t1, t2的索引保存到\
*key_buffer_1*\ 。

.. sourcecode:: sql

    CACHE INDEX t1, t2 IN key_buffer_1;

通过\ ``SHOW STATUS``\ 和\ ``SHOW VARIABLES``\ 监视键缓冲区的使用情况和性能。主
要指标有：\ ``缓存命中率``\ 和\ ``缓存使用率``

缓存命中率
-----------

.. math::

    \begin{equation}
        \% = \frac{key_read}{key_read_requests}\times 100
    \end{equation}

缓存使用率
----------
.. math::

    \begin{equation}
        \% = \frac{key_blocks_unused * key_cache_block_size}{key_buffer_size}\times 100
    \end{equation}

分配键缓存大小时，了解MyISAM索引的大小比较有帮助，使用下面命令可以计算索引文件的
大小。

.. sourcecode:: bash

    du -sch `find /var/lib/mysql -name '*.MYI'`

MyISAM数据块的大小
------------------
键数据块的大小非常重要，因为它会影响MyISAM、操作系统和文件系统间的交互。如果键数
据块太小，就会导致写入排队的情况。如果键数据块大小与操作系统相匹配，可以避免写入
等待。

``myisam_block_size``\ 变量控制着键缓存块的大小，也可以在\ ``CREATE TABLE``\ 或\
``CREATE INDEX``\ 语句中为每一个键定义\ ``KEY_BLOCK_SIZE``\ 选项来控制键的大小。


InnoDB缓冲池
============


线程缓存
========
``thread_cache_size``\ 定义了MySQL能在缓存中保存的线程数量。如果一个新的连接被创
建且缓存中有线程，MySQL就会从缓存中取出一个并赋给它连接；当连接关闭时，MySQL会回
收线程存放到缓存中，如果（线程）缓存中已经存满，则会将其销毁。

通过观察变量\ ``thread_created``\ 的值可以确定线程缓存是否足够大。如果每秒创建的
线程数量少于10个，缓存的大小就是够的。

对大多数情况而言，非常巨大的线程缓存是没有必要的。每个在缓存中的线程通常会消耗
256KB内存。

表缓存
=======
表缓存有助于复用资源。如：当查询要求访问MyISAM表时，MySQL就可以从缓存中取出一个
文件描述符，而不是打开一个文件。

表缓存被分为两个部分：一部分为打开表）；另一部分为表的定义。分别由变量\
``table_open_cache``\ 和\ ``table_definition_cache``\ 定义。表的定义（解析过的
.frm文件）和其它资源（如文件描述符）是隔离的。打开的表仍然是基于每个线程、每个使
用的表。而表的定义是全局的，可以在所有连接中共享。

如果状态\ ``opened_tables``\ 的值很大或者不断上升，就说明缓存不够大，应该增加系
统变量\ ``table_cache``\ （或\ ``table_open_cache``\ ）的值。将表缓存变得很大的
唯一坏处就是有很多MyISAM表的时候，会导致较长的关闭时间，因为要冲刷键数据块，而且
表要被标记为不再打开。同样也会导致\ ``FLUSH TABLES WITH READ LOCK``\ 需要较长时
间才能完成。

如果收到MySQL不能打开更多文件的提示，需要在配置文件中使用\ ``open_files_limit``\
来增加可打开文件数。


InnoDB数据字典
================


MyISAM I/O调优
==============
此处I/O主要指的是数据写入磁盘，由于写缓存的存在，MySQL的性能会大大提高，但是也会
引起一些风险，如突然断电，系统崩溃，缓存中的数据没有写入到磁盘，就可能导致数据丢
失，即使是恢复，也可能需要相当长的时间。

通常MyISAM在每次写入之后就会把索引的变化刷写到磁盘上。如果打算对一个表进行很多改
变，那么把它们组成一个批处理会快很多。

``LOCK TABLES``\ 可以将写延迟到对表解锁，所以可以用来精确的控制延迟写入。

变量\ ``delay_key_write``\ 可以控制MyISAM键的延迟写入。可以取下面三个值：

*   ``OFF`` MyISAM每次写入后就把键缓冲区中修改过的数据块刷写到磁盘上，除非表被\
    ``LOCK TABLES``\ 锁定。
*   ``ON``  键延迟写入被开启。不过只对使用\ ``DELAY_KEY_WRITE``\ 项创建的表有效
*   ``ALL`` 所有MyISAM表都使用键延迟写入。

键延迟写入对性能提高有一定帮助，但不会带来飞跃。

选项\ ``myisam_recover_options``\ 控制着MyISAM查找和修复错误的方式，取值如下：

*   ``DEFAULT``\ 或不设置   MySQL会修复所有被标记为崩溃及没有标记为干净关闭的表
*   ``BACKUP``  MySQL会将数据文件备份到一个.bak文件，可以方便随后检查
*   ``FORCE``   即使.MYD丢失一行，恢复也会继续
*   ``QUICK``   

选项\ ``myisam_use_mmap``\ 可以开启使用内存映射打开数据文件。


InnoDB I/O调优
==============
InnoDB使用事务日志来减少提交事务的开销。每次事务提交时，并不会将缓存池写入到磁盘
，而是记录到事务日志中。InnoDB最终还是要将数据变化写入到数据文件，它是通过后台线
程智能的将数据变化写入文件（因为每次事务，不同事务的写入操作可能会进行随机I/O，
而该线程会将事务中的I/O以高效的顺序I/O写入至数据文件）。

事务日志也使用了缓存，即日志缓存。大小由变量\ ``innodb_log_buffer_size``\ 来控制
，通常大小为1-8M，对大型事务，可能需要实际调整。在InnoDB数据发生变更时，它会将变
化写入至日志缓存（内存）中，当缓存满、事务提交或每一秒任一条件满足，InnoDB会将日
志缓冲区的写入磁盘日志文件中。

事务日志文件的大小由\ ``innodb_log_file_size``\ 和\
``innodb_log_files_in_group``\ 两个变量来控制。默认日志文件为2个，大小均为5M。对
高负载，256M应该可以满足需求，总大小上限为4G。日志文件是以循环的方式写入的，即当
记录到达日志底部，则会从顶部重新开始，但是不会覆盖没有写入至数据文件的记录。

如果想改变日志文件的大小，需要干净的关闭MySQL，确认日志中所以记录已写入到数据文
件，然后移走原日志文件，重新配置\ ``innodb_log_file_size``\ 启动服务器，检查错误
日志，确认没有问题后删除原日志文件。

那么日志缓存又是如何写入到日志文件的呢？前面已经提到过，在三种情况下会将日志缓存
写入磁盘：\ *缓存满、事务提交或每秒*\ 。这是通过变量\
``innodb_log_at_trx_commit``\ 来控制的，它可以取下面三个值：

*   ``0``   将日志缓存写入到日志文件中，且每秒写入一次，有事务提交时不进行操作
*   ``1``   将日志写到日志文件中，且在事务提交时把缓存写入到\ **持久存储**\ 中
    （确保写入硬盘）。默认设置
*   ``2``   每次事务提交时将日志缓存写入到日志文件中，但不进行清理。InnoDB每秒会
    清理一次。MySQL崩溃时，事务不会丢失，但是数据存储崩溃、掉电则可能丢失事物

注意写入到文件和写入到持久存储是有差别的。（系统缓存的存在）

另外变量\ ``innodb_flush_log_at_atx_commit``\ 也对I/O有着非常大的影响。

变量\ ``innodb_flush_method``\ 控制InnoDB如何与文件系统进行交互。

.. todo::

    innodb_flush_method的介绍。


InnoDB表空间
------------
InnoDB将数据保存在表空间中。使用变量\ ``innodb_data_file_path``\ 定义表空间文件
，\ ``innodb_data_home_dir``\ 定义表空间文件所在的目录。如：

.. sourcecode:: ini

    innodb_data_home_dir = /var/lib/mysql
    innodb_data_file_path = ibdata1:1G;ibdata2:1G;ibdata3:1G[:autoextend[:max:2G]]

InnoDB会依序向这些文件中写入数据，第一个写满了再写第二个……所以将这些文件分部存储
至不同磁盘上并没有效果。在最后一个文件后面，我们可以使用\ ``autoextend``\ ，当表
空间耗尽（即所以文件都写满）后，最后一个文件会自动增长，不过文件大小是只增不减的
。为了防止文件过大，可以使用\ ``max:2G``\ 来设定一个上限。

变量\ ``innodb_file_per_table``\ 可以使用InnoDB为每一表使用一个文件（在数据库目
录中以“tbl_name.idb”保存数据），这样带来一些便利的同时会浪费更多的空间。

变量\ ``innodb_max_purge_lag``

.. todo::
    
    查手册补全

双写缓存。变量\ ``innodb_doublewrite``\ 控制。


Binlog的写入
------------
``sync_binlog``\ 控制MySQL如何将binlog写入到磁盘。默认为\ ``0``\ ，即MySQL不会进
行任何刷写操作，何时把日志持久化至存储设备由操作系统来控制。

变量\ ``expire_logs_days``\ 用来设置日志的有效期。不要使用\ *rm*\ 删除binlog，因
为你不知道binlog是否已经同步至slave服务器。可以使用\ ``PURGE MASTER LOGS``\ 删除
binlog。


参考资源
========
1.  《高性能MySQL》

..  [#query_cache]  `How the Query Cache Operates
    <http://dev.mysql.com/doc/refman/5.5/en/query-cache-operation.html>`_
