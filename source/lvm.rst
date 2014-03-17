LVM基础知识
***********

基本概念
========
如果我们单块磁盘最大容量为1TB，但是有一个数据库文件有5个TB怎么办？在\ **分区**\
这个概念下是无法做到的，我们无法将小的分区（文件系统）合在一起使用。LVM可以做到，
前面也介绍过，LVM中的LV（逻辑卷）与物理磁盘的大小是完全无关的，可以将不同大小的\
物理盘合并到一起最终构建一个大的LV，那要怎么去实现呢，下面我们将介绍。

使用LVM(Logic Volume Manager)可以灵活方便的管理物理磁盘。你是否遇到某个磁盘分区\
空间不足，给你的文档存放规范带来不便。使用LVM可以轻松的解决这个问题。逻辑卷实际\
上是将物理磁盘虚拟化，这样逻辑卷的大小就不受物理卷大小的限制。并且它还有其它优\
点：

1.  *灵活的容量* 当使用逻辑卷时，可在多个磁盘间扩展文件系统，因为您可以将磁盘和\
    分区集合成一个逻辑卷。
2.  *重新设定存储池大小* 您可以使用检单的软件命令增大或者减小逻辑卷的大小，而无\
    需对所在磁盘设备重新格式化或者重新分区。
3.  *在线数据重新定位* 要部署更新、更快或者更有弹性的存储子系统，以便您可以在系\
    统活跃时移动数据。数据可以在磁盘正在使用时进行重新分配。例如，您可以在删除\
    一个热交换磁盘之前将其清空。
4.  *方便设备命名* 逻辑存储卷可在用户定义的组群中进行管理，这些组群可按您的要求\
    进行命名。
5.  *磁盘条带* 您可以创建一个可在两个或者更多磁盘间条状分布数据的逻辑卷。这可大\
    幅度提高吞吐量。
6.  *镜像卷* 逻辑卷为您提供了一个方便配置数据镜像的方法。
7.  *卷快照* 使用逻辑卷，您可以提取设备快照，这样可在持续备份或者在不影响真实数\
    据的情况下测试修改效果。

使用LVM需要掌握几个新的概念：\ ``物理卷，卷组，逻辑卷``\ 。对详细说明请参考参考\
资料中列出的内容。

构建LVM
========
简单流程：

*   使用命令 \ ``pvcreate``\ 创建\ ``PE``
*   使用命令 \ ``vgcreate``\ 创建\ ``VG``
*   使用命令 \ ``lvcreate``\ 创建\ ``LV``

再用\ ``mkfs``\ 格式化LV，即可以使用LV存储文件数据了。

一个新的磁盘（存储器）插入到系统后会被识别为一个新的独立的文件系统，这类似于我\
们将一块大磁盘分割为几个小的文件系统（比如windows下经常分的C, D, E......等等分\
区）。如果你想将一个小的文件系统加入到LVM中，首先需要在这个小的文件系统上建立\
PE(physical extend)，命令：\ ``pvcreate``\ 。

PV管理
=======
相关命令有：
 pvchange   pvcreate   pvmove     pvresize   pvscan
 pvck       pvdisplay  pvremove   pvs

创建
------
使用命令\ ``pvcreate``\ 创建PE，\ ``pvcreate``\ 的操作会破坏原分区（文件系统）\
中的数据。

.. sourcecode:: bash

    pvcreate /dev/sda1 /dev/sda2 /dev/sda3
    # pvcreate后接分区，即可在分区上创建PE

查看PV/PE
----------
命令\ ``pvdisplay``\ 可以用来查看已创建PE的PV（物理卷）的属性，如，大小，PE大小\
，所属VG等等。例如：

1.  默认使用选项-v，显示详细信息

.. sourcecode:: bash

    pvdisplay [-v]
    #   Scanning for physical volume names
    # --- Physical volume ---
    # PV Name               /dev/sda10
    # VG Name               vg
    # PV Size               972.65 MiB / not usable 4.65 MiB
    # Allocatable           yes
    # PE Size               4.00 MiB
    # Total PE              242
    # Free PE               61
    # Allocated PE          181
    # PV UUID               fGisVN-LA7s-MPJW-F5ZI-gYqC-SS0O-UPNzPI
    # 
    # --- Physical volume ---
    # PV Name               /dev/sda11
    # VG Name               vg
    # PV Size               972.65 MiB / not usable 4.65 MiB
    # Allocatable           yes
    # PE Size               4.00 MiB
    # Total PE              242
    # Free PE               223
    # Allocated PE          19
    # PV UUID               I0HjTy-Bcjf-oZG1-4hTR-CxVq-vHF1-DTvzyn
    # 
    # "/dev/sda9" is a new physical volume of "972.65 MiB"
    # --- NEW Physical volume ---
    # PV Name               /dev/sda9
    # VG Name
    # PV Size               972.65 MiB
    # Allocatable           NO
    # PE Size               0
    # Total PE              0
    # Free PE               0
    # Allocated PE          0
    # PV UUID               dhL0cH-wS6w-dscI-PXRv-dRLS-aXaN-tzn08w

2.  也可以使用选项“ \ ``-s|--short``\ “显示精简信息。如：

.. sourcecode:: bash

    pvdisplay -s
    # Device "/dev/sda10" has a capacity of 244.00 MiB
    # Device "/dev/sda11" has a capacity of 892.00 MiB
    # Device "/dev/sda9" has a capacity of 972.65 MiB

3.  也显示指定分区的信息。

.. sourcecode:: bash

    pvdisplay [option] <pv_path>

增加物理卷
----------
如果你存放数据的文件系统，随着数据增加，空间不够，需要增加磁盘，怎么把新的磁盘\
空间增加到原来的文件系统中呢？

确认*/dev*目录下可以发现新加磁盘，然后使用\ ``fdisk``\ 命令将新磁盘分区（亦可不\
分）。

最后用\ ``pvcreate *PhysicalVolume*``\ 在新磁盘上建立PV。

移除物理卷
----------
如果需要更换某个磁盘，那怎么办呢？

卷组(VG)管理
============
相关命令有:
 vgcreate, vgs, vgdisplay
 vgreduce, vgextend, vgremove, vgexport, vgimport
 vgmerge, vgsplit等等

创建VG
--------
当完成创建PE之后就可以在相应的分区上建立VG了。使用命令\ ``vgcreate``\
来创建VG，如：

.. sourcecode:: bash

    # 创建一个VG，其名字为vgName。
    vgcreate vg-test /dev/sda9
    # Volume group "vg-test" successfully created

.. note::

    1.  VG可以使用多个分区，也正是因为这样，LVM才能把多个小的分区（文件系统）联合起
    来形成一个大的文件系统
    2.  另外，一个分区只能属于一个VG

查看VG信息
-----------
与查看PV/PE信息一样，使用命令\ ``vgdisplay``\ 来查看VG的信息。例如：

.. sourcecode:: bash

    vgdisplay
    #  --- Volume group ---
    #  VG Name               vg
    #  System ID
    #  Format                lvm2
    #  Metadata Areas        2
    #  Metadata Sequence No  26
    #  VG Access             read/write
    #  VG Status             resizable
    #  MAX LV                0
    #  Cur LV                5
    #  Open LV               1
    #  Max PV                0
    #  Cur PV                2
    #  Act PV                2
    #  VG Size               1.89 GiB
    #  PE Size               4.00 MiB
    #  Total PE              484
    #  Alloc PE / Size       200 / 800.00 MiB
    #  Free  PE / Size       284 / 1.11 GiB
    #  VG UUID               DXKStQ-rGBh-kVSa-PDy2-eFvg-FLAx-kELRZX

可以查看到卷组vg的相关信息，其中大部分都可以在创建VG时设定，不过一般我们都会使\
用默认值，除非你有特殊的要求。

其实查看PE, VG的信息还有其它命令\ ``pvs, vgs``\ ，这两个命令主要用于生成报告信\
息，便于SA了解所需的信息。

分割合并卷组
------------

逻辑卷(LV)管理
==============
相关命令:
 lvcreate, lvresize, lvextend, lvreduce, lvremove等等

逻辑卷类似于系统中的分区/dev/sda1, /dev/sda2等等，是用来存储数据的。

创建LV
-------
与创建PE，VG类似，创建LV使用命令\ ``lvcreate``\ 来创建LV。创建一个分区当然要指\
明这个分区的大小了，还要给一个名字便于查找使用。

.. sourcecode:: bash

    lvcreate -L +200M -n lv-test vg-test

上面的命令将在卷组vg-test中创建一个名为lv-test，200M大小的LV。选项*-L*后接LV的\
大小，单位可以是K(b), M(b), T(b), P(b), E(b)；选项*-n*后接LV的名称，最后为已存\
在的 VG名称（即要在哪个VG上建立LV）。

查看LV属性
----------
使用命令\ ``lvdisplay``\ 来查看LV的相关信息。用法与\ ``pvdisplay, vgdisplay``\
类似。如：

.. sourcecode:: bash

    lvdisplay /dev/vg/vg-test
    #  --- Logical volume ---
    #  LV Name                /dev/vg/vg-test
    #  VG Name                vg
    #  LV UUID                DwWf1w-0UMN-61WD-rHF1-pZtM-uec5-z7mGQW
    #  LV Write Access        read/write
    #  LV Status              available
    #  # open                 1
    #  LV Size                500.00 MiB
    #  Current LE             125
    #  Segments               1
    #  Allocation             inherit
    #  Read ahead sectors     auto
    #  - currently set to     256
    #  Block device           253:9


线性逻辑卷
----------

条状逻辑卷
----------

镜像逻辑卷
----------

快照卷
------

LVM重要操作
===========
使用LVM来构建系统
-----------------
调整LV（逻辑分区）大小
----------------------
**情 境**\ ：挂载于/usr下的分区LV（逻辑分区）lv_usr空间快被消耗殆尽，怎么办？

**解决步骤**\ ：

*   确认VG（卷组）是否有足够的剩余空间。如果剩余空间不足，可以缩小其它剩余空间较
    多的LV，也可以增加新磁盘
*   使用命令\ ``lvresize``\ 增加\ *lv_usr*\ 空间

    .. sourcecode:: bash

        lvresize -L +increaseNumber /dev/VolGroup/lv_usr

*   使用命令\ ``resize2fs``\ 扩大逻辑分区\ *lv_usr*\ 上的文件系统，使其与分区大
    小一致。此时系统可能提示你要先运行\ ``fsck``\ 检查一下文件系统

    .. sourcecode:: bash

        e2fsck -f /dev/VolGroup/lv_usr
        resize2fs /dev/VolGroup/lv_usr

*   使用命令\ ``lvs``\ 确认扩容成功。

.. note::

    关键是：运行\ ``lvresize``\ , \ ``resize2fs``\ 的先后顺序。扩容一定要先运行\
    ``lvreize``\ 扩大分区，再运行\ ``resize2fs``\ 扩大文件系统

增加或更换磁盘
--------------
情境：由于某个块磁盘年代比较久远，性能较差，需要更换新的磁盘，利用LVM如何在系统
不停机的情形下更换硬盘？
    
现在情况为：系统中有一块磁盘/dev/sda，在其上有一个分区/dev/sda1，/dev/sda1为VG（
卷组）VGroup中的P，其中有很多LV
    
完成步骤：

*   将新的硬盘经测试后安装到系统上并进行分区（此处我们假定新磁盘为/dev/sdb1）
*   使用命令\ ``pvcreate``\ 将新的磁盘加入到PV（物理卷）

    .. sourcecode:: bash

        pvcreate /dev/sdb1

*   使用命令\ ``vgextend``\ 将/dev/sdb1加入到卷组VGroup中

    .. sourcecode:: bash

        vgextend VGroup /dev/sdb1

*   在线使用命令\ ``pvmove``\ 将/dev/sda1上的数据转移/dev/sdb1

    .. sourcecode:: bash

        pvmove /dev/sda1 /dev/sdb1

*   使用命令\ ``vgreduce``\ 将磁盘/dev/sda1从卷组VGroup中移除

    .. sourcecode:: bash

        vgreduce VGroup /dev/sda1

*   使用命令\ ``pvremove``\ 将磁盘/dev/sda1从PV中移除

    .. sourcecode:: bash

        pvremove /dev/sda1

*   将原磁盘拆下即可

利用快照卷进行在线备份
----------------------

性能问题
--------

参考资料
=========
