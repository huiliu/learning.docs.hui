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

客户端可以通过http://devhome/repos/hello\ 来访问SVN仓库

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


参考资料
==========
.. [#]  运行\ ``svnadmin load``\ 时，如果主仓库（“\ */repos/hello*\ ”）中的文件\
        有加锁，会出错并中断当前操作。需要删除仓库中的锁才能继续。

        .. code-block:: bash

            # 删除SVN仓库中的锁
            svnadmin lslock /repos/hello > helloLocks
            for f in `grep Path hellolocks |awk '{print $2}'`
            do
                svnadmin rmlocks /repo/rhsrc $f
            done

        利用上面的命令可以将SVN仓库中的锁信息保存在文件中将其删除
