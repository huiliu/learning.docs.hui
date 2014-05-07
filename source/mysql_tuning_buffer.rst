MySQL通用调优原则
******************
如果服务器使用单一存储引擎，调优相对容易的多。如，只使用MyISAM表，就可以完全禁止
InnoDB；反之类似。

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
参考资源
========
1.  《高性能MySQL》
