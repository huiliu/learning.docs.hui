MySQL数据库的备份
*****************

备份类型
=========
物理备份与逻辑备份
------------------

**物理备份**\ 直接备份MySQL Server的数据文件，适用于数据量大，比较重要并且需要\
快速恢复的数据。其特点有:\ [#]_

1.  比逻辑备份快（为什么？）
2.  进行备份操作时需要MySQL Server停止服务或者禁止改变数据库内容的操作
3.  备份粒度
4.  可以方便备份与数据库相关的配置，日志信息

**逻辑备份**\ 通过向MySQL Server发生请求以获取数据库的结构和内容信息并进行保存\
。其特点有：

1.  比物理备份慢（为什么？）
2.  备份文件相对较大（为什么？）
3.  备份、恢复的粒度更小
4.  备份后的数据与机器无关，便于恢复
5.  需要MySQL Server处于运行状态

在线备份和离线备份
------------------


本地备份和远程备份
------------------

执行SQL语句\ ``SELECT ... INTO OUTFILE``\ 可以将数据输出到MySQL Server上的某个\
文件中（无论在本地还是远程执行此语句）。


快照备份
---------
利用文件系统（LVM, ZFS）的快照功能（snapshot）可以对数据库所在的文件分区进行快\
照备份。（需要对文件系统进行合理的设计）


全备份和增量备份，以及PIT(Point-in-Time)恢复
------------------------------------------------


表维护
------

备份方法
========
直接备份数据库文件
------------------
对于使用\ ``MyISAM``\ 引擎的数据表，可能通过直接复制其数据文件（\ ``*.frm,
*.MYD, *.MYI``\ ）来进行备份。为了保证备份时数据不会发生改变，应该停止服务器运\
行或对相关的数据表进行锁表操作：\ [#]_

.. sourcecode:: sql

    FLUSH TABLES tbl_list WITH READ LOCK;

另外也可以使用工具\ ``mysqlhotcopy``\ 完成相同的任务。由于存储引擎\ ``InnoDB``\
的特性（并不一定并数据存放在相应的数据库目录下，另外即使服务器没有更新硬盘数据\
，也可能在修改缓存于内存中的数据而没有将其回写到硬盘）,\ ``mysqlhotcopy``\ 并不\
适用于使用\ ``InnoDB``\ 引擎的表进行备份。

将数据备份到分隔符文件
-----------------------
``SELECT * INTO OUTFILE 'file_name' FROM tbl_name``\ 可以将数据导出到MySQL
Server上，注意保存文件\ ``file_name``\ 不能存在（安全方面的考量），另外此方法只\
能保存数据，不会导出表结构。

使用命令\ ``mysqldump``\ 和\ ``--tab``\ 选项可以完成相同的操作，同时可以导出表\
结构（包含CREATE TABLE语句）。

使用\ ``LOAD DATA INFILE``\ 或命令\ ``mysqlimport``\ 可以将导出的数据恢复至数据\
库。


增量备份
--------
开启\ ``binlog``\ 功能后可以对数据库进行增量备份。进行增量备份时，必须首先刷新\
``binlog``\ ：\ ``FLUSH LOGS``\ 。另外也可以使用命令：\
``mysqldump --flush-logs ``\ 或\ ``mysqlimport --flush-logs``\ 。

使用从库
--------


恢复受损表
----------
对于使用\ ``MyISAM``\ 引擎的表，如果发生损坏，可以使用\ ``REPAIR TABLE``\ 或者\
``myisamchk -r``\ 来进行恢复，有99.9%的几率修复数据。

使用文件系统快照
----------------
一般可以执行下面步骤：

1.  从客户端登陆并执行：\ ``FLUSH TABLES WITH READ LOCK``
2.  从另外一个Shell执行文件系统快照操作
3.  从客户端执行解锁：\ ``UNLOCK TABLES``
4.  将快照文件拷贝备份


备份策略
=========
一般备份策略是，建立一个完整备份，然后定期进行增量备份。\ [#]_

对于使用\ ``InnoDB``\ 引擎的数据表，使用\ ``mysqldump``\ 进行备份时加上选项\
``--single-transaction``\ 可以保证备份时数不会发生变化。对于全部使用\ ``INFILE``
的数据库，可以使用下面命令进行全备份：\

.. sourcecode:: bash

    mysqldump --single-transaction --all-databases > backup_20140308.sql

对非事务型引擎，如\ ``MyISAM``\ 备份时必须加上读锁以保证备份时数据不会发生变更：

.. sourcecode:: sql

    FLUSH TABLES WITH READ LOCK;

注：\ *执行\ ``FLUSH``\ 语句时，如果系统正在执行一个耗时的操作，会短暂阻塞直到\
完成相关操作*


执行增量备份需要MySQL Server开启二进制日志(binlog)功能。开启binlog后，服务器每\
次重启都会新建一个binlog，并将所有的数据变更写入到binlog中。也可能手动执行\
``FLUSH LOGS``\ 语句刷新binlog，或者执行命令\ ``mysqladmin flush-logs``\ 刷新\
binlog。数据目录下的\ ``*.index``\ 中包含了当前目录下的所有binlog列表。

``mysqldump``\ 也有刷新binlog的选项，所有可以使用下面方法来建立一个完整备份：

.. sourcecode:: bash

    mysqldump --single-transaction --flush-logs --master-data=2 --all-databases
    > backup_20140308

全备份+增量备份的一个实践是：

1.  利用上面的\ ``mysqldump``\ 命令（带\ ``--flush-logs``\ 选项）建立一个完整备\
    份;
2.  定期运行命令\ ``mysqladmin flush-logs``\ 以刷新binlog，然后备份相应的binlog\
    即可

对于数据量较大的网站，binlog将占相当大的空间，所以需要定期进行清理。运行SQL语句\
``PURGE BINARY LOG``\ 或者运行命令\ ``mysqldump``\ 时加上选项\
``--delete-master-logs``\ 也可以删除二进制日志。如果建立了主从同步，在删除主库\
上的binlog时需要小心，因为binlog的内容可能还没有同步至从库。\ ``PURGE BINARY
LOG``\ 删除binlog前会执行相关检查。

参考资料
========
.. [#]  http://dev.mysql.com/doc/mysql-backup-excerpt/5.5/en/backup-types.html
.. [#]  http://dev.mysql.com/doc/mysql-backup-excerpt/5.5/en/backup-methods.html
.. [#]  http://dev.mysql.com/doc/refman/5.5/en/backup-policy.html
