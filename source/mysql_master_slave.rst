MySQL配置Master-Slave
**********************

三步实现Master-Slave
=====================

最简单的Master-Slave只需要三步（前提是Master和Slave上的数据完全一样）。

1.  配置一个Master服务器

    1.  配置文件
        
        .. code-block:: ini

            [mysqld]
            # ......
            log-bin         =   master-bin
            log-bin-index   =   master-bin.index
            server-id       =   1

    2.  创建用于复制的帐户

        .. code-block:: sql

            GRANT REPLICATION SLAVE ON *.* TO 'replication'@'192.168.%.%' IDENTIFIED BY 'replication'

2.  配置一个Slave服务器

    1.  配置文件

        .. code-block:: ini

            [mysqld]
            # ......

            server-id       =   2
            relay-log-index =   slave-relay-bin.index
            relay-log       =   slave-relay-bin

3.  将Slave连接到Master

    1.  在命令配置

        .. code-block:: sql

            CHANGE MASTER TO MASTER_HOST='master', MASTER_PORT=3306, MASTER_USER='replication', MASTER_PASSWORD='replication';

            START SLAVE;

建立新的Slave
===============
建立全新Slave的关键在：\ **在最小影响情况下如何获取一份Master的数据？**\
即克隆Master。

方法一 **mysqldump**
----------------------
使用\ `mysqldump`\ 可以将Master中的数据完整的导出，并在Slave上恢复，操作简单。\
但是此方法存在严重的问题，因为\ `mysqldump`\ 操作需要锁定数据的写操作，会导致数\
据库一定时间内无法写入数据，相关服务可能会受到影响。

1.  刷新并锁定数据库，防止再导出数据时数据库发生变化

    .. code-block:: sql

        FLUSH TABLES WITH READ LOCK;

2.  记录Master上binlog的位置。如下面bin-log文件为: “\ *mysql-bin.000015*\ ”，\
    position为: “\ *245*\ ”

    .. code-block:: sql

        SHOW MASTER STATUS;
        -- *************************** 1. row ***************************
        --                        File: mysqld-bin.000015
        --                    Position: 245
        --                Binlog_Do_DB: 
        --            Binlog_Ignore_DB: 
        -- 1 row in set (0.00 sec)

3.  利用命令\ `mysqldump`\ 导出Master上的数据，完成后解锁Master

    ::

        mysqldump --all-databases; --host=master-1 > backup-2013xxxx.sql

    .. code-block:: sql

        UNLOCK TABLES;

4.  在Slave上恢复Master的数据

    ::
        
        mysql --host=slave-1 -u root -p < backup-2013xxxx.sql

5.  连接Master-Slave，并启动Slave

    .. code-block:: sql

        CHANGE MASTER TO MASTER_HOST='master', MASTER_PORT=3306, MASTER_USER='replication', MASTER_PASSWORD='replication', MASTER_LOG_FILE='mysql-bin.000015', MASTER_LOG_POS=245;

        START SLAVE;

方法二：从Slave上克隆数据
-------------------------
如果本来就存在着一个Master-Slave关系，就可以方便的在不影响任何服务的情况下建立一个新的Slave——从Slave上复制数据至新的Slave。操作类似于从Master复制数据。

1.  停止Slave并刷新锁定数据库

    .. code-block:: sql

        STOP SLAVE;
        SHOW SLAVE STATUS;
        -- *************************** 1. row ***************************
        --                Slave_IO_State: Waiting for master to send event
        --                   Master_Host: 192.168.122.1
        --                   Master_User: replication
        --                   Master_Port: 3306
        --                 Connect_Retry: 60
        --               Master_Log_File: mysqld-bin.000015
        --           Read_Master_Log_Pos: 245
        --                Relay_Log_File: db-server-relay-bin.000015
        --                 Relay_Log_Pos: 530
        --         Relay_Master_Log_File: mysqld-bin.000015
        --              Slave_IO_Running: Yes
        --             Slave_SQL_Running: Yes
        --               Replicate_Do_DB: wiki
        --              ......
        --          Exec_Master_Log_Pos: 245
        --              Relay_Log_Space: 1113
        --              Until_Condition: None
        --               Until_Log_File: 
        --                Until_Log_Pos: 0
        --           Master_SSL_Allowed: No

        -- 锁定数据库
        FLUSH TABLES WITH READ LOCK;

2.  记录在Slave上Master的\ **bin-log**\ 执行到的位置。主要关注两个字段：“\ **Relay_Master_Log_File**\ ”和“\ **Exec_Master_Log_Pos**\ ”。
3.  同方法一中复制Slave中的数据并在新的Slave上恢复。
4.  将新的Slave连接到Master。其中“\ **MASTER_LOG_FILE**\ ”和“\ **MASTER_LOG_POS**\ ”的值为第二步所记录的值。
    
    .. code-block:: sql
        
        CHANGE MASTER TO MASTER_HOST='master', MASTER_PORT=3306, MASTER_USER='replication', MASTER_PASSWORD='replication', MASTER_LOG_FILE='mysql-bin.000015', MASTER_LOG_POS=245;

        START SLAVE;

其它备份MySQL数据的方法
========================


故障说明
=========
运行主从同步时，出现如下情况：

.. code-block:: text

    Slave_IO_Running: Connecting
    Slave_SQL_Running: Yes

从网上查询了一下原因，大家提到的有：

1.  log_file_pos不正确
2.  同步帐户设置不正确
3.  网络问题

全都试着重新完成一遍，结果没有解决，看看日志发现是“\ **binlog找不到**\ ”，\
再回头看看主库，binlog被清除了。

一句话：\ ``出错的原因可能有千百种，看日志才是王道。``

参考资料
==========
1. 高可用的MySQL——构建健壮的数据中心
