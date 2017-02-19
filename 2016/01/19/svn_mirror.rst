SVN仓库安全之镜像备份
*************************



.. author:: default
.. categories:: devops
.. tags:: devops, deploy
.. comments::
.. more::



软件需求
=========
本文经过在CentOS 5.5_X86上实验操作通过，所需相关软件有：

*   操作系统：CentOS 5.5 X86
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

    如何快速建立SVN镜像仓库

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

.. warning::

    运行\ ``svnsync init ... ...``\ 时可能会出现如下错误：

    .. sourcecode:: text

        svnsync: DAV request failed; it's possible that the repository's pre-revprop-change hook either failed or is non-existent
        svnsync: At least one property change failed; repository is unchanged
        svnsync: Error setting property 'sync-lock':
        could not remove a property
    
    这个错误说明缺少\ *hook*\ “pre-revprop-change”。只需要镜像仓库的hook目录下\
    创建一个可执行脚本文件\ ``pre-revprop-change``\ 返回0即可，如果此脚本返回值\
    不为0会出现下面的提示：

    .. sourcecode:: text

        svnsync: DAV request failed; it's possible that the repository's pre-revprop-change hook either failed or is non-existent
        svnsync: At least one property change failed; repository is unchanged
        svnsync: Error setting property 'sync-lock':
        版本属性改变 被 pre-revprop-change 钩子阻塞(退出代码 1) 输出：
        Changing revision properties other than svn:log is prohibited

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

安装好上面的hook后，当用户向\ *http://devhome/repos/hello*\ 提交数据，当前提交\
会自动同步到\ *http://devhome/backup/hello*\ 。如果SVN的访问需要授权，则必须提\
供用户名和密码，且保证正确（小心有些系统设置了密码的有效期）

案例
======
主仓库所在硬盘故障，将SVN服务由镜像仓库顶上，SVN的提交将直接被写入镜像。主仓库\
硬盘修复后（数据无损失），将提交至镜像仓库的数据导入主仓库，恢复主－镜像架构。

切换至镜像仓库
================
尽量不要向镜像仓库提交数据，不得不使用镜像仓库时，先对镜像仓库进行一个完整备份，
以备再次建立镜像备份。因为镜像仓库本来就是设计为只读仓库的，从镜像恢复主仓库可\
能会出现各种意外，后续将介绍一些实际经验。

将镜像仓库作为主仓库接收用户提交数据时，还需要将镜像仓库的\ ``UUID``\ 修改为主\
仓库的\ ``UUID``\ 。

.. sourcecode:: bash

    # UUID文件位于文件repos_name/db/uuid中
    REPOS_PATH=/repos
    for dir in `ls $REPOS_PATH`
    do
        file="${REPOS_PATH}/${dir}/db/uuid"
        cat $file
    done


从镜像仓库恢复主仓库
====================
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


问题说明
========
在实际运行过程中会出现各种奇怪的问题，集结如下：

1.  运行命令\ ``svnsync sync http://xxx.xxx.xxx/backup/xxxx``\ 提示错误：

    .. sourcecode:: text

        Transmitting file data .svnsync: Corrupt representation '9890 0 12903 ..

    猜想应该是revision 9890有问题，将镜像仓库的相当修订号改为小于9890，再运行\
    同步命令成功。

    .. note::

        调整镜像仓库修订号需要修改文件\ ``db/current``\ 和\ ``db/revprops/0/0``\


2.  运行同步命令\ ``svnsync sync http://xxxx/backup/xxx``\ 时，提示错误：

    .. sourcecode:: text

        Transmitting file data ....svnsync: Server sent unexpected return value (423 Locked) in response to PUT request for '/backup/

    提示无法获取锁，将镜像仓库目录\ ``db/locks``\ 下的文件删除，再同步即可。

3.  运行命令\ ``svnsync sync http://xxxx/backup/xxx``\ 出现下面的错误：

    .. sourcecode:: text

        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        Failed to get lock on destination repos, currently held by 'devhome:8b8d326b-34b1-4fe1-8a81-a33e841d5047'
        svnsync: Couldn't get lock on destination repos after 10 attempts

    打开SVN仓库中的文件\ ``db/revprops/0/0``\ 你会发现最后几行有\ ``sync-lock``\
    和上面的UUID值，所以只要将此lock删除掉就可以。比较安全的方法是使用命令删除：
    [#ref_lock]_

    .. sourcecode:: bash

        svn propdel --revprop -r0 svn:sync-lock file:///path/to/the/repository

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
            for f in `grep Path hellolocks | awk '{print $2}'`
            do
                svnadmin rmlocks /repo/rhsrc $f
            done

        利用上面的命令可以将SVN仓库中的锁信息保存在文件中将其删除

.. [#ref_lock]  `svnsync - couldn't get lock on destination repos 
                <http://stackoverflow.com/questions/4077601/svnsync-couldnt-get-lock-on-destination-repos>`_
