Speed Up Firefox
****************

通过将Firefox的配置文件和临时文件目录存放至内存中，可以加快Firefox的启动迅速，减
少上网时对硬盘的读写，可能在一定程度上加速网页渲染。

在系统启动和关闭时执行一些命令就可以自动实行这个功能。如在Gentoo中，在目录“\
*/etc/local.d*\ ”添加合适的脚本并且正确命名它们，就能定制开关机时的一些额外的工作。

1.  **scriptName.start**   系统启动时执行的脚本
2.  **scriptName.stop**    系统关机时执行的脚本

系统启动时将文件拷入内存
========================
在目录 */etc/local.d* 下添加一个文件，命名为“\ **firefox.start**\ ”：

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


修改Firefox的Cache路径
=======================
在Firefox的地址栏输入“\ **about:cache**\ ”可以查看浏览器当前的缓存存放位置。
可以修改相应的参数值将缓存移到你所希望的地方，如内存磁盘。可以减少对物理磁盘的\
读写。操作如下：

1.  在Firefox地址栏中输入“\ **about:config**\ ”，会出现一个警告，直接无视进入就\
    好。
2.  在页面的中的搜索栏中查找“\ **browser.cache.disk.parent_directory**\”，修改\
    其值，设定为你所期望的路径，如Linux下可以设为："/tmp"。如果没有\ **browser.\
    cache.disk.parent_directory**\ 这一项（多半是没有），单击右键自己创建一个，\
    并设定相应值就可以了。
3.  操作同第二步，只是要修改\ **browser.cache.offline.parent_directory**\ 的设\
    定值。
4.  关闭重新打开Firefox，在地址栏输入“\ **about:cache**\ ”，你就会发现缓存目录\
    已经修改了。

.. 此部分google，百度可以找到N多资料，也比较混乱，在此不列出参数信息。
