Speed Up Firefox
****************

通过将Firefox的配置文件和临时文件目录存放至内存中，可以加快Firefox的启动迅速，减
少上网时对硬盘的读写，可能在一定程度上加速网页渲染。

在系统启动和关闭时执行一些命令就可以自动实行这个功能。如在Gentoo中，在目录"\
*/etc/local.d*"添加合适的脚本并且正确命名它们，就能定制开关机时的一些额外的工作。

|       script.start   系统启动时执行的脚本
|          script.stop    系统关机时执行的脚本

系统启动时将文件拷入内存
========================
在目录 */etc/local.d* 下添加一个文件，命名为"**firefox.start**"：

.. literalinclude:: _static/code/firefox.start
    :language: bash


系统关闭时从内存拷贝数据至硬盘
==============================
在目录 */etc/local.d* 下添加一个文件，命名为"firefox.\ **stop**"：

.. literalinclude:: _static/code/firefox.stop
    :language: bash

安装软件包"**profile-sync-daemon**"
===================================
软件包"**profile-sync-daemon**"是用Perl写的一套脚本，功能与上面的脚本类似，不过\
更为强大。它支持多种不同的浏览器，而且设计为服务的形式，更加方便用户使用。在\
Gentoo下可以直接安装：

.. sourcecode:: bash

    emerge -qv www-misc/profile-sync-daemon

其它系统类似，也可以直接\ `下载`_\ 安装。

.. _下载: https://wiki.archlinux.org/index.php/Profile-sync-daemon



    
