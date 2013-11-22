SVN仓库安全之镜像备份
*************************

软件需求
=========
本文经过在CentOS5.5_X86上实验通过，相关软件有：

*   操作系统：CentOS5.5 X86
*   mod_authz_ldap-0.26-9.el5
*   mod_dav_svn-1.6.15-0.1
*   subversion-1.6.15-0.1
*   httpd-2.2.3-43.el5.centos.3

建立SVN仓库（主）
==================
使用命令\ ``svnadmin create``\ 可以方便建立SVN仓库，然后在Apache的配置文件中设\
定好URL访问路径，客户端就可以通过HTTP访问SVN服务器。

*   建立仓库

    .. code-block:: bash
    
        cd /repos
        svnadmin create hello
        chown -R apache:apache hello
    
        # 如果服务器开启了SELinux的功能，还需要调整SELinux相关的标签
        chcon -R -t httpd_sys_content_t repos

*   设定Apache

    .. code-block:: apache

        LimitXMLRequestBody 0
        LimitRequestBody    0

        <Location /repos>
            DAV svn
            SVNParentPath   /repos
            SVNPathAuthz    off
            # ... ...
        </Location>

*   重新加载Apache配置

    .. code-block:: bash

        /etc/init.d/httpd reload

客户端可以通过http://devhome/repos/hello\ 来访问SVN仓库。

如果SVN仓库非常庞大，数据达到十几G，几十G，\ ``svnsync``\ 的效率将非常低，同步\
完仓库可能要十几个小时甚至更长。所以可能通过伪造欺骗的方式来建立镜像仓库。

.. todo::

    如果快速建立SVN镜像仓库

建立主SVN仓库的镜像(Mirror)
============================
建议镜像仓库的前期步骤与主仓库一致，当建立好镜像仓库后需要使用命令\ ``svnsync``\
将镜像仓库与主仓库同步、建立连接。假定镜像仓库的路径为：\ */backup/hello*\ ，访\
问URL为：http://devhome/backup/hello\ 。镜像仓库必须是一个全新的SVN仓库，没有进\
行任何提交。

.. code-block:: bash

    svnsync init http://devhome/backup/hello http://devhome/repos/hello
    svnsync sync http://devhome/backup/hello

``svnsync init``\ 会在主（源）－镜像（目标）仓库间建立连接，\ ``svnsync sync``\
将主仓库中的数据同步到镜像仓库。如果数据比较多，会需要相当长的时间。


安装hooks－实时镜像
====================
完成上面两步后，就可以手动通过\ ``svnsync sync``\ 来同步主库我镜像仓库的数据，\
使其保持一致。更为方便的是在主仓库中安装一个post-commit的hook，（关于钩子hook的\
使用请查看其它资料）当客户端向主仓库完成一次提交后，自动将数据同步到镜像仓库。

*   在“\ */repos/hello/hooks*\ ”目录下建立一个文件“\ **post-commit**\ ”并将其设\
    为可执行。这个文件可以是任何形式的可执行文件，在此处使用Shell，最简单的文件\
    内容为：
 
    .. code-block:: bash

        #!/bin/bash

        svnsync sync http://devhome/backup/hello --non-interactive --no-auth-cache --username usersync --password passwd

安装好上面的hook后，当用户向http://devhome/repos/hello提交数据，当前提交会自动\
同步到http://devhome/backup/hello

案例
======
主仓库所在硬盘故障，将SVN服务由镜像仓库顶上，SVN的提交将直接被写入镜像。主仓库\
硬盘修复后（数据无损失），将提交至镜像仓库的数据导入主仓库，恢复主－镜像架构。

切换至镜像仓库
================



导入数据至主仓库
=================
由于故障时将镜像仓库用作主仓库接受客户端的数据提交，所以当修复的主仓库重新上线\
时，镜像仓库的数据比主仓库的更新一些，所以必须将提交到镜像仓库的数据重新导回主\
仓库才能重新恢复主－镜像备份功能。

首先我们尝试使用\ ``svnsync``\ 命令来同步：

.. code-block:: bash

    svnsync sync http://devhome/repos/hello

使用上面的命令会收到下面的错误：\

.. code-block:: text

    svnsync: Destination HEAD xxx is not the last merged revision; have you\
    committed to the destination without using svnsync

上面就是说没有使用\ ``svnsync``\ 向镜像仓库提交了数据，导致镜像仓库的数据比主仓\
库的数据要新。所以需要将镜像仓库中的新数据\ **dump**\ 出来导入到主仓库。

.. code-block:: bash

    svnadmin dump /repos_backup/hello -r 主库revisionNumber+1 --incremental | svnadmin load /repos/hello

运行上面的命令导出导入数据时，可能会出错中断操作。\ [#]_

重新恢复主－镜像功能
=======================
镜像仓库数据导入回主仓库后，主仓库和镜像仓库的数据就完全一致（请确认）。此时运\
行命令\ ``svnsync sync http://devhome/backup/hello``\ 会收到错误：

.. code-block:: text

    svnsync: Destination HEAD (11295) is not the last merged revision (11297);
    have you committed to the destination without using svnsync?

从错误推断，镜像仓库应该是不允许提交数据，向镜像仓库提交数据会导致主－镜像无法\
同步，所以需要重新恢复同步信息。有以下几个欺骗SVN的方法：

*   修改\ */backup/hello/db/current*\ 的值为同步中断时的值，然后重新运行命令\
    ``svnsync sync http://devhome/backup/hello``\ 。运气好的话可以重新同步成\
    功。\ 也有可能会出错：

    .. code-block:: text

        Transmitting file data .svnsync: Corrupt representation '25773 0 20806
        212480 0a6b7637ee622c6f0b2cb8fd8ecb9f48
        b5c5091ce33b04b5b7cb747b046d0e1114c7a7cc 25772-jwm/_6'

    如果出现上面的错误，请使用命令\ ``svnadmin verify /backup/hello -r revNum``\
    检查修订号为\ *revNum*\ 的提交数据是否正常，极有可能有问题。

*   使用命令\ ``svnadmin recover /backup/hello``\ 恢复SVN信息，查看打开“\
    */backup/hello/db/revprop/0/0*\ ”如下：

    .. code-block:: text

        K 8
        svn:date
        V 27
        2009-09-02T04:01:29.647149Z
        K 26
        svn:sync-currently-copying
        V 5
        25775
        K 17
        svn:sync-from-url
        V 26
        http://devhome/repos/mdrez
        K 18
        svn:sync-from-uuid
        V 36
        4c74e609-66f4-4995-99c0-adb26f254cac
        K 24
        svn:sync-last-merged-rev
        V 5
        25774
        END

    将“\ **svn:sync-currently-copyin**\ ”和“\ **svn:sync-last-merged-rev**\ ”下\
    面的修订号，如“25775”，“25774”修改为“\ */backup/hello/db/current*\ ”中的值，\
    然后进行同步：\ ``svnadmin verify /backup/hello -r revNum``\ 。同步将顺利完\
    成。但是可以出现其它一些错误。在此不一一列举，出现错误后将\ ``db/current``\
    和\ ``db/revprop/0/0``\ 中的修订号再修改小一点，重新运行\ ``svnsync``\ 命令\
    ，一般可以消除问题。（需要深入的了解一下）

*   用上面的方法基本可以保留原镜像，主仓库不变而恢复主－镜像架构，但是实际中可\
    能会出现各种错误。\ **建议完成重建一个镜像仓库。**


参考说明
==========
.. [#]  运行\ ``svnadmin load``\ 时，如果主仓库（“\ */repos/hello*\ ”）中的文件\
        有加锁，会出错并中断当前操作。如：

        .. code-block:: text

        svnadmin: Cannot verify lock on path '... ...'; no username available

        需要删除仓库中的锁才能继续。

        .. code-block:: bash

            # 删除SVN仓库中的锁
            svnadmin lslock /repos/hello > helloLocks
            for f in `grep Path hellolocks |awk '{print $2}'`
            do
                svnadmin rmlocks /repo/rhsrc $f
            done

        利用上面的命令可以将SVN仓库中的锁信息保存在文件中将其删除
