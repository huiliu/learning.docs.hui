MySQL数据库的备份
*****************



.. author:: default
.. categories:: database, mysql
.. tags:: database, mysql, dba
.. comments::
.. more::



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

从备份恢复数据
--------------
从上面通过全备份+增量备份的备份数据恢复：

1.  先从全备份恢复

    .. sourcecode:: bash

        mysql < backup_20140308

2.  然后依时间序从增量备份的binlog恢复数据

    .. sourcecode:: bash

        mysqlbinlog mysqld-binlog.00001 mysqld-binlog.00002 | mysql

3.  对于从最后一个增量备份时间点到MySQL Server崩溃点间的数据，依不同备份策略可\
    能会丢失

mysqldump
==========

Dumping Data in SQL Format
--------------------------
默认情况下导出数据存储为SQL语句

*   ``--all-databases | -A``    **导出所有数据库**

    .. sourcecode:: bash

        mysqldump --all-databases > dump.sql

*   ``--databases | -B db1 db2``    **导出指定的数据库**

    .. sourcecode:: bash

        mysqldump --databases db1 db2 db3 > dump.sql

*   ``--add-drop-database`` **添加**\ ``DROP DATABASE``\ **语句**

    .. sourcecode:: bash

        mysqldump --add-drop-database --all-databases > dump.sql

``mysqldump``\ 使用选项\ ``--all-databases, --databases``\ 时，在导出数据时会\
主动添加\ ``CREATE DATABASE``\ 和\ ``USE``\ 语句，即主动创建相应的数据库。如果\
需要清除数据库中的原数据，可以增加选项\ ``--add-drop-database``\ 。

注意导出单个数据库时下面的差异：

1.  ``mysqldump --databases db1 > dump_db1.sql``
2.  ``mysqldump db1 > dump_db1.sql``

方法一会在导出数据中添加\ ``CREATE DATABASE``\ 语句；而方法二则不会。方法二导出\
的数据可以方便的导入到与原数据库不同名的数据库。

恢复由\ ``mysqldump``\ 导出的数据可以直接运行\ ``mysql < dump.sql``\ 或者\
``mysql> source dump.sql``\ 来导入数据。导入数据时需要注意的是是否包含\ ``CREATE
DATABASE``\ 语句，需要根据需要进行一些额外的操作。

Dumping Data in Delimited-Text Format
--------------------------------------
命令\ ``mysqldump``\ 使用选项\ ``--tab=dir_name``\ 可以将指定的数据库导出为分隔\
符格式的数据文件，存放于目录\ ``dir_name``\ 中的两个文件：\ **tbl_name.txt,
tbl_name.sql**\ 。\ txt文件中存放的是表中的数据；sql文件中表结构信息（\ ``CREATE
TABLE``\ 语句等）。如：

.. sourcecode:: bash

    shell> mysqldump --tab=/tmp db1

上面的命令将数据库\ **db1**\ 中的所有表导出到\ */tmp*\ 目录下，每一张表对应两个\
文件（tbl_name.txt, tbl_name.sql）。\ ``txt``\ 文件实际上是由MySQL Server执行\
``SELECT ... INTO OUTFILE``\ 所生成，用户必须有\ ``FILE``\ 权限，另外，如果存在\
相应的txt文件，会提示出错。MySQL Server将\ ``CREATE``\ 等定义发送给\
``mysqldump``\ 并写入到sql文件，所以sql文件的所有者（及其它权限）属于执行\
``mysqldump``\ 命令的用户。

最好在本地执行\ ``mysqldump --tab=dir_name``\ 命令，目录\ **dir_name**\ 必须同\
时存在于本地和远程服务器（MySQL Server）。txt文件将被Server写在服务器上，而sql\
文件将被保存在本地（执行mysqldump命令的主机）。

如果需要定制分隔符文件的格式还有一些其它选项用于配置。\ [#]_

由于\ ``mysqldump --tab``\ 备份的数据是由两个文件组成，所以恢复时与SQL格式备份\
略有不同：\ [#]_

1.  导入\ *tbl_name.sql*\ 文件，建立相应的数据表\ ``mysql db1 < tbl_name.sql``
2.  导入\ *tbl_name.txt*\ 中的数据，可以有以下不同的方法：

    *   直接使用命令：\ ``mysqlimport [options] db1 tbl_name.txt``\ 。如果在导\
        出数据时，使用了自定义格式，在import时也需要加上相应选项。
    *   在mysql shell下：

        .. sourcecode:: bash

            mysql> USE db1;
            mysql> LOAD DATA INFILE 'tbl_name.txt' INOT TABLE t1;

Dumping Stored Programs
-----------------------
对于数据库中的\ ``stored procedures, functions, triggers, events``\ 的备份，有\
一些额外的选项：

*   ``--events``\ ：导出events
*   ``--routines``\ ：导出stored procedures, functions
*   ``--triggers``\ ：导入triggers（默认）

如果想显式的禁止导出，相应的选项有：

*   ``--skip-events``
*   ``--skip-routines``
*   ``--skip-triggers``

Dumping Table Definitions and Content Separately
-------------------------------------------------
有时候可能只想导出表结构或者表数据，\ ``mysqldump``\ 同样提供了相应的功能选项：

*   ``--no-data``\ ：仅导出表结构，即\ ``CREATE``\ 语句
*   ``--no-create-info``\ ：仅导出表数据，即\ ``INSERT INTO``\ 语句

例如：

.. sourcecode:: bash

    shell> mysqldump --no-data test > dump-defs.sql
    shell> mysqldump --no-create-info test > dump-data.sql

在准备数据库版本时，最好分别寻出表结构和数据，然后分别在新版本的服务器上进行恢\
复。由于表结果部分不包括数据，可以很快的导入，发生错误时也便于检查修复。确认结\
构恢复正常后再导入数据，并确认正常。


Point-in-Time Recovery - mysqlbinlog
====================================
通过全备份/增量备份，我们可以通过工具\ ``mysqlbinlog``\ 从二进制日志中逐步恢复\
数据。执行SQL语句\ ``SHOW BINARY LOGS``\ 可以查看二进制日志列表；\ ``SHOW MASTER
STATUS``\ 可以查看当前正在使用日志文件。

可以通过命令\ :code:`mysqlbinlog binlog_file | mysq -u root -p`\ 来恢复数据，也\
可以通过命令\ :code:`mysqlbinlog binlog_file | less`\ 来查看binlog中的内容。

另外需要注意的是，从binlog恢复数据时，如果是恢复多个文件，应该在单个连接中完成\
多个文件的恢复，即：

.. sourcecode:: bash

    mysqlbinlog binlog.00001 binlog.00002 binlog.00003 | mysql -u root -p
    # 或者
    mysqlbinlog binlog.00001 > log.sql
    mysqlbinlog binlog.00002 >> log.sql
    mysqlbinlog binlog.00003 >> log.sql
    mysql -u root -p -e 'source log.sql'

依据时间恢复
------------
``mysqlbinlog``\ 有两个选项\ ``--start-datetime``\ 和\ ``--stop-datetime``\ 可\
以设定从某个时间点开始恢复，或者恢复至某个时间点。如：

.. sourcecode:: bash

    # 恢复至2005年4月20号上午10点
    shell> mysqlbinlog --stop-datetime="2005-04-20 9:59:59" /var/lib/mysql/binlog.123456 | mysql -u root -p
    # 从2005年4月20号上午10点开始恢复
    shell> mysqlbinlog --start-datetime="2005-04-20 9:59:59" /var/lib/mysql/binlog.123456 | mysql -u root -p

依据事件点来恢复
----------------
为了能够准确的恢复到某个日志位置，需要确定日志中期望事件的\ `log_pos`\ 。

.. sourcecode:: bash

    # 释放出binlog中的内容
    shell> mysqlbinlog /var/lib/mysql/binlog.00001 > log.sql
    查看log.sql中查看\ `log_pos`\ 然后找到合适位置
    shell> mysqlbinlog --start-position=368315 /var/log/mysql/bin.123456 | mysql -u root -p

需要注意的是恢复后的数据，相关的时间均为日志中的时间。

MyISAM表的维护与恢复
====================

由\ ``MyISAM``\ 引擎的特点决定对表数据进行更新整理可以减少使用空间，提高访问性\
能，而且可以减少表数据出现故障的几率。所以使用\ ``MyISAM``\ 引擎的表需要制定周\
期性计划对表进行优化整理。

使用\ ``MyISAM``\ 引擎的表在对应的数据库目录下存在三个文件：

+---------------+----------------------------+
| File          | Purpose                    |
+---------------+----------------------------+
| tbl_name.frm  | Definition (format) file   |
+---------------+----------------------------+
| tbl_name.MYD  | Data file                  |
+---------------+----------------------------+
| tbl_name.MYI  | Index file                 |
+---------------+----------------------------+

``myisamchk``\ 命令
--------------------
*   ``外部锁``\：如果MySQL Server禁用了外部锁（默认设定），使用\ ``myisamchk``\
    命令是不安全的。如果mysqld与myisamchk在同一张表上工作，会引起此表的数据损坏\
    。此时最安全的做法是停止MySQL Server服务再执行\ ``myisamchk``\ 命令；如果可\
    以确定mysqld没有在某张表上工作，可以先运行命令\ ``mysqladmin flush-tables``\
    ，再运行\ ``myisamchk``\ 命令。
*   如果外部锁（external lock）被mysqld开启，可以在任何时候运行\ ``myisamchk``\
    以检查数据表。（为什么？锁）
*   通常情况下\ ``myisamchk``\ 命令通过逐行复制数据文件\ ``.MYD``\ 来进行检查和\
    修复工作，当修复结束时，会删除原来的MYD文件，将新的文件命名为原文件。

检查错误
^^^^^^^^^^
下列选项可用于检查\ ``MyISAM``\ 表的错误

*   ``myisamchk tbl_name``  可以发现99.99%的问题，如果仅仅是数据文件的错误则无\
    法发现。\ ``-s (slient)``\ 不输入正常信息

*   ``myisamchk -m  tbl_name``  首先会检查\ *索引条目*\ ；然后会计算并核对每行中
    的KEY的\ *校验和*

*   ``myisamchk -e tbl_name``   (-e "extened check")会彻底的检查所有数据。对于\
    数据比较多的表会消耗较长时间。当发现第一错误后将会终止。选项\ ``-v``\ 将会\
    显示更加详细的信息，且会显示最多20个错误才终止检查。
*   ``myisamchk -e -i tbl_name``    与上面的命令类似，选项\ ``-i``\ 会显示一些\
    额外的统计信息。

修复\ ``MyISAM``\ 表
^^^^^^^^^^^^^^^^^^^^
.. warning::

    **进行任何修复操作前必须进行备份! 如果使用了主从复制，请先停止！**

常见的错误包括：
*   *tbl_name.frm*\ 被锁无法修改
*   找不到文件\ *tbl_name.MYI*
*   无法预期的文件结尾
*   记录文件损坏
*   处理表时发生错误\ *nnn*

通过命令\ ``perror nnn``\ 可以获取详细的错误描述：

.. sourcecode:: bash

    perror 132 134 135 136
    # OS error code 132:  Operation not possible due to RF-kill
    # MySQL error code 132: Old database file
    # MySQL error code 134: Record was already deleted (or record file crashed)
    # MySQL error code 135: No more room in record file
    # MySQL error code 136: No more room in index file

错误135(*No more room in record file*)和(*No more room in index file*)可以使用\
``ALTER TABLE``\ 来增大表选项中的\ ``MAX_ROWS``\ 和\ ``AVG_ROW_LENGTH``\ 的值来\
修复：

.. sourcecode:: sql

    ALTER TABLE tbl_name MAX_ROWS=100000 AVG_ROW_LENGTH=10000;

通过\ ``SHOW CREATE TABLE``\ 可以查看表的相关选项值。

对于其它一些错误（非135/136）需要使用\ ``myisamchk``\ 进一步修复。修复一般分为\
四步：（\ **在进行修复前必须先将MySQL Server关闭。**\ ）

1.  **检查表**

    运行命令\ ``myisamchk [-e | others options] *.MYI``\ 选项\ *-s*\ 可以减少不\
    必要的输出。使用 \ *--update-state*\ 选项，\ ``myisamchk``\ 会对检查过的表\
    进行标记。

    对于一般错误进入第二步进行修复；对于无法预知的错误（如\ *内存溢出*\ ，\
    *myisamchk崩溃*\ ）跳至第三步进行修复。
2.  **简单安全的修复**

    *   首先通过命令\ ``myisamchk -r -q tbl_name``\ 尝试快速修复。这种方法将尝\
        试修复索引文件而不处理数据文件。如果修复失败，则进行下一步；

    *   备份数据文件
    *   使用\ ``myisamchk -r tbl_name``\ 进行修复，此操作将从数据文件中删除不正\
        的数据，并重建索引文件。

    *   如果上一步失败，使用\ ``myisamchk --safe-recover tbl_name``\ 进行修复。\
        安全修复模式会使用一种比较老的方法来修复。

    如果操作过程中出现\ *内存溢出，myisamchk崩溃*\ 请执行第三步进行恢复。
3.  **困难的修复**

    如果走到这一步通常是：

    *   索引文件的最初16KB损坏或包含不正确的信息
    *   如果索引文件丢失，则必须新建一个索引文件：
        *   将数据文件备份至安全位置
        *   使用表描述文件创建新的数据文件和索引文件
            
            .. sourcecode:: sql

                USE db_name;
                SET autocommit=1;
                TRUNCATE TABLE tbl_name;
                quit;

        *   将数据文件复制回来一份，覆盖新的数据文件
    *   回到第二步执行\ ``myisamchk -r -q``\ 。另外也可用\ ``REPAIR TABLE
        tbl_name USE_FRM``\ 来进行修复。
4.  如果需要执行此步进行修复，说明表描述文件(.FRM)已经损坏，这种情况几乎不会发\
    生，因为表描述文件在数据表建立好后就不会再改变。修复方法如下：

    *   从备份中恢复表描述文件（.FRM）返回执行第三步进行修复；如果索引文件也可\
        从备份中恢复，返回第二步进行修复；

    *   如果没有备份但是知道相应的表是如果创建的，可以在其它库中建立一个相同的\
        表，然后将其.frm, .MYI文件复制一份与数据文件.MYD组合成完整的表，然后回\
        至第二步进行修复操作，尝试重建索引文件。

    *   如果啥也没有，那就废了。

``myisamchk``\ 还有一些选项可以加速修复操作：\ ``sort-buffer-size,
key-buffer-size``\ 。

MyISAM表的优化
^^^^^^^^^^^^^^
使用\ ``OPTIMIZE TABLE``\ 也可以进行\ ``MyISAM``\ 的优化操作，通常包含\ *表修复,
KEY分析，索引树排序*\ 等操作，经过优化可以提高KEY的查找速度。

``myisamchk``\ 命令用于优化的常用选项有：

*   ``-r | --recover``  修复\ ``MyISAM``\ 表中几乎所有的问题（除KEY不唯一外），\
    回收被浪费的空间；
*   ``-a | --analyze``  通过分布分析改善\ ``JOIN``\ 性能。使用join优化器选择最\
    佳的表连接和索引顺序
*   ``-S | --sort-index``   对索引块排序，优化查找性能，提高用索引搜索表的速度
*   ``-R | --sort-records=index_num``   应用指定的索引对数据行进行排序，使得数\
    据聚集的更好，提高基于范围（range-based）的\ ``SELECT``\ 和\ ``ORDER BY``\
    操作的速度


MyISAM的周期维护
-----------------
优化\ `MyISAM`\ 既可以使用SQL语句\ ``CHECK TABLE, REPAIR TABLE, OPTIMIZE TABLE,
ANALYZE TABLE``\ 也可以通过\ ``myisamchk``\ 命令。

需要注意：执行\ ``myisamchk``\ 命令时，相应的数据表不可以操作。

正常情况下，MySQL的表只需进行很少维护，如果\ ``MyISAM``\ 表更新的大量可变大量的\
内容（\ ``VARCHAR, BLOB, TEXT``\ 类型的字段），或者删除的大量的数据，这时可以通\
过\ ``OPTIMIZE TABLE``\ 进行优化。

MySQL Server自动维护
^^^^^^^^^^^^^^^^^^^^
如果MySQL Server的配置文件中开启了选项\
``--myisam-recover-options=[options,...]``\ ，此选项有五个值可选（并可以组合使\
用）：\ ``OFF, DEFAULT, BACKUP, FORCE, QUICK``\ 。选项值为空等同于\ ``DEFAULT``\
，而选项值为：""等同于\ ``OFF``\ 。开启此功能后，mysqld每次打开\ `MyISAM`\ 表时\
，都会检查表\ *是否被标记为损坏或者非正常关闭*\ ，如果是，mysqld将会检查并修复\
表。从文档表述来看，开启此功能对MySQL Server的性能有一定影响。

+---------+--------------------------------------------------------------------+
| Option  |                         Descripton                                 |
+---------+--------------------------------------------------------------------+
| OFF     | 关闭mysqld的自动维护\ ``MyISAM``\ 表的功能                         |
+---------+--------------------------------------------------------------------+
| DEFAULT | Recovery without backup, forcing or quick checking                 |
+---------+--------------------------------------------------------------------+
| BACKUP  | 如果数据在恢复时发生变化，备份tbl_name.MYD为tbl_name-datetime.BAK  |
+---------+--------------------------------------------------------------------+
| FORCE   | 即使会丢失\ `.MYD`\ 中的数据，也执行恢复                           |
+---------+--------------------------------------------------------------------+
| QUICK   | 如果没有删除块的操作，Do not check the rows in the table.          |
+---------+--------------------------------------------------------------------+


crontab计划任务
^^^^^^^^^^^^^^^
在crontab中添加一条计划任务定期检查是个不错的选择：

.. sourcecode:: vim

    35 0 * * 0 /path/to/myisamchk --fast --silent /path/to/datadir/*/*.MYI

也可以通过执行SQL语句\ ``OPTIMIZE TABLE``\ 来优化。

小结
----
*   对\ ``MyISAM``\ 进行定期维护以降低故障发生的可能性。
*   修复前关停MySQL Server，备份数据。
*   可以使用SQL语句\ ``OPTIMIZE, CHECK, REPAIR, ANALYZE TABLE``\ 或者\
    ``myisamchk``\ 进行维护和故障修复。


参考资料
========
.. [#]  http://dev.mysql.com/doc/mysql-backup-excerpt/5.5/en/backup-types.html
.. [#]  http://dev.mysql.com/doc/mysql-backup-excerpt/5.5/en/backup-methods.html
.. [#]  http://dev.mysql.com/doc/refman/5.5/en/backup-policy.html
.. [#]  http://dev.mysql.com/doc/refman/5.5/en/mysqldump-delimited-text.html
.. [#]  http://dev.mysql.com/doc/refman/5.5/en/reloading-delimited-text-dumps.html
