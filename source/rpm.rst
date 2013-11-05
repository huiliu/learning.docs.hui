RPM软件包
***********

私有软件仓库
=============
在公司的软件部署中，只需要部分软件包，而且版本是固定的，如此，如果使用公用的软\
件仓库，比较难以保证版本的要求，为此最好是将要使用软件包存放在一起，构成一个私\
有的软件仓库。

按以下步骤建立一个私有仓库：

1.  收集所需的软件及其依赖包。需要注意的是由于一般Linux发行版都会安装大量软件包\
    ，所以如果在直接下载，依赖链可能并不完整。建议可以在最小安装版的CentOS上进\
    行。使用下面的命令可以从公有仓库下载相应软件包：

    .. code-block:: bash

        yum --downloadonly --downloaddir=/tmp/repos

2.  使用\ `createrepo`\ 命令来创建仓库metadata。

    .. code-block:: bash

        yum install createrepo
        createrepo /tmp/repos
        # 在目录/tmp/repos上会自动生成仓库需要的元数据

3.  配置HTTP或FTP服务对外提供仓库数据
4.  在客户机上配置使用私有仓库。



参考资料
==========
.. [#]  http://wiki.centos.org/HowTos/CreateLocalRepos
