磁盘配额管理
************

对于Linux服务器，一般都是多用户的，如何防止某一个或一些用户将系统资源占用太多\
呢，\ ``ulimit``\ 命令可以进行一些限制。对于一个文件存储服务器，怎么给用户分配\
一定额度的空间呢？总不能把磁盘分为N个分区一个用户分一块吧。Linux提供了一个工具\
``quota``\ 控制每个用户或工作组的磁盘使用量。

命令有：
 ``quotacheck, setquota, edquota``
 ``quotastats, repquota, warnquota``
 ``quotaoff, quotaon``
 ``quota, quotasync, quota_nld, rpc.rquotad``

.. note:

    *   quota仅能对文件系统中用户的用量进行控制，而不是针对目录（当然，如果某目\
        录下挂载着一个文件系统，是可以进行磁盘用量限制的）；
    *   需要内核支持；且使用quota功能的文件系统，挂载时需要\
        ``usrquota, grpquota``\ 这两个挂载选项。

启用quota功能
=============
*   首先确认你的内核支持quota，一般发行版Linux内核都支持quota功能，如果是自己手\
    动编译，注意要选择支持quota。
*   第二步，安装quota软件包。具体方法请参照你所用的系统。
*   第三步，在磁盘的挂载选项中添加上\ ``usrquota, grpquota``\ 。使得文件系统支\
    持quota。
*   最后，运行命令\ ``quotacheck -a``\ 自动检查确认哪些系统支持quota。

开机自开启
----------
如果你想开机后quota功能自动开启

*   只需要将挂载选项\ ``usrquota, grpquota``\ 加入到\ ``/etc/fstab``\ 中相应的\
    文件系统
*   然后将\ ``quota``\ 服务设定为开机自动运行即可。

简单举例
=========
首先使用LVM创建了一个500M的实验分区scale

.. sourcecode:: bash

    # lvs
    LV     VG   Attr   LSize   Origin Snap%  Move Log         Copy%  Convert
    scale  vg   -wi-ao 500.00m

然后将该分区挂载到\ */tmp/scale*\ 下。

生成记录文件
------------
运行命令： \ ``quotacheck -avug``\ 。目录/tmp/scale（文件系统）中生成两个文件:\
*aquota.user, aquota.group*\ （注意这个文件是由``quotacheck``生成的，而不是手动\
创建的）。\ *aquota.user, aquota.group*\ 这两个文件是用来存放用户配额相关信息的\
，并不是文本格式。

开启和关闭quota功能
-------------------
使用命令\ ``quotaon, quotaoff``\ 来手动开启或关闭磁盘配额功能。如果你想开机自动\
运行，应该将quota服务设定为开机自运行。

.. sourcecode:: bash

    # 开启磁盘配额管理功能
    quotaon -vug filesystem
    quotaon -avug
    # 关闭磁盘配额管理功能
    quotaoff -vug filesystem
    quotaoff -avug

设置服务开机自运行的方法请根据你的系统设定。

为用户设定配额值
-----------------
最为激动的一个环节到了，怎么来为用户，工作组来设定配额呢？命令``edquota``将为您服务。

.. sourcecode:: bash

    # 对sheldon进行配额限制
    edquota -u sheldon
    # 将会打开如下格式的一个文件：
    Disk quotas for user sheldon (uid 1000):
      Filesystem                   blocks       soft       hard     inodes     soft     hard
      /dev/mapper/vg-scale          20480      10240      30720          3        0        0

.. warning::

    上面的文件是我已经编辑过的，如果对没有对用户进行过设置，则所有项均为0.

这个文件为一个表格形式，分为七列：

*   第一列为文件系统，即哪个文件系统上sheldon个用多少空间。
*   第二列为blocks.此列由系统自动计算得到的。请勿修改。
*   同样第五列inodes与第二列一样，由系统自动计算得到的。
*   第三，六列均为soft限制，即当用户所用磁盘量超过这个值后，会触发一些系统动作。
*   第四，七列为hard限制，即用户使用磁盘量达到这个值之后，无法再写入文件。请特\
    别注意。

从上面我们可以发现，\ ``quota``\ 可以对用户使用磁盘量的两个指标进行限制－\
``block``\ 和\ ``inode``\ ，关于两个值的意义请查看文件存储部分。

.. warning::

    使用命令\ ``edquota``\ 编辑配额时，``数字必须与列名右对齐``，否则会提示格式\
    错误而设置失败。

同样\ ``quota``\ 也支持对工作组磁盘用量的限额，只需要使用选项\ ``-g``\ 替代\
``-u``\ 就可以了。例如：

.. sourcecode:: bash

    # 对bigbang小组进行限制
    edquota -g bigbang
    Disk quotas for group bigbang (gid 1000):
      Filesystem                   blocks       soft       hard     inodes     soft     hard
      /dev/mapper/vg-scale          20480    102400      307200          3        0        0

其格式与作用与对user的设定完全一致。

如果运行一个文件服务器，可能对很多用户的配额是完成一样的，比较说sheldon，harwod\
与leonard都只可以使用100M空间，你设定好sheldon的，然后再设定leonard的，再……如果\
有N个人一样，你一个一个的去设定那不得麻烦S啊。你一定要相信Linux一定有"偷懒"的方\
法。下面就是一个：

.. sourcecode:: bash

    # 将sheldon的配额设定copy一份给leonard，这样他们的就完全一样了，也不用再编辑
    edquota -p sheldon -u leonard

Easy吧。

设定宽限期
----------
前面我们知道配额有一个\ ``soft``\ ，一个\ ``hard``\ 限制，这两者有什么区别呢？

当用户磁盘用量超过\ ``soft``\ 限制之后，会触发一个宽限期（grace-time），如果在\
宽限期内用户占用磁盘量没有下降到soft设定值下，soft将变为hard。超出的数据会被怎\
么样还不清楚，正在实验。

下面的命令用于设定grace-period（宽限期）：

.. sourcecode:: bash

    # 默认是设定针对用户的宽限期
    edquota -t
    # 加上参数``-g``可以设定工作的宽限期
 edquota -t -g
    # 使用选项``-T``可以针对某个用户或工作组进行设定，如果没有特别设定，则使用设
    # 定（即``-t``的设定值）。
    edquota -T -u sheldon
    edquota -T -g bigbang

批量设定磁盘配额
-----------------
命令\ ``setquota``\ 不同于\ ``edquota``\ 通过编辑方式设定配额，\ ``setquota``\
可以方便的使用命令／参数模式来批量的设定用户的磁盘配额。请看命令说明：

.. sourcecode:: bash

    setquota [ -rm ] [ -u | -g ] [ -F quotaformat ] name block-softlimit block-hardlimit inode-softlimit inode-hardlimit -a | filesystem
    setquota [ -rm ] [ -u | -g ] [ -F quotaformat ] [ -p protoname ] name -a | filesystem
    setquota -b [ -rm ] [ -u | -g ] [ -F quotaformat ] -a | filesystem
    setquota -t [ -m ] [ -u | -g ] [ -F quotaformat ] block-grace inode-grace -a | filesystem
    setquota -T [ -m ] [ -u | -g ] [ -F quotaformat ] name block-grace inode-grace -a | filesystem

例如，设定sheldon只能使用/home文件系统100M，上限为200M，可以如此设定：

.. sourcecode:: bash

    setquota -u sheldon 100000 200000 0 0 /home

同样可以设定宽限期：

.. sourcecode:: bash

    setquota -t -u sheldon 7 0 /home

警告提示
----------
如果仅仅是限额而没有提示，这很可能会造成用户数据溢出丢失，Linux怎么会允许这样情\
况发现了。为了方便提醒用户，\ ``qutoa``\ 提供了一个命令\ ``warnquota``\ 用户提\
醒用户。

如果你写入的文件直接超出了hard限定，程序会提示你写入错误，磁盘配额超出，你所写\
入的数据将是不完整的，切记！

.. sourcecode:: bash

    $ dd if=/dev/zero of=test bs=1M count=30M
    dd: writing `test': Disk quota exceeded
    14+0 records in
    13+0 records out
    14581760 bytes (15 MB) copied, 0.0170676 s, 854 MB/s

``dd``\ 命令提示“\ *Disk quota exceeded*\ ，再看本来计划写入30M数据，结果只写入\
了15M，写入数据不完整的，超出部分的数据将会丢失。

命令\ ``warnquota``\ 的作用是：检查所有文件系统中的配额控制，如果有用户的磁盘用\
户达到的\ ``soft``\ 的限制，就会给用户发送一封邮件提醒。注意\ ``warnquota``\ 不\
会自动执行，如果你想定期检查，请用\ ``cron``\ 功能。

``warnquota``\ 需要一个配置文件\ ``/etc/warnquota.conf``\ 。内容摘要如下，在配\
置文件中，定义邮件的相关信息，如：邮件主题、内容，发送者，签名等等

.. sourcecode:: text

    MAIL_CMD        = "/usr/sbin/sendmail -t"
    FROM            = "root@CCTV"
    # but they don't have to be:
    SUBJECT         = Hey, user, clean up your account!
    CC_TO           = "sysadm@example.com"
    # If you set this variable CC will be used only when user has less than
    # specified grace time left (examples of possible times: 5 seconds, 1 minute,
    # 12 hours, 5 days)
    # CC_BEFORE = 2 days
    SUPPORT         = "support@example.com"
    PHONE           = "(123) 456-1111 or (222) 333-4444"
    # Text in the beginning of the mail (if not specified, default text is used)
    # This way text can be split to more lines
    # Line breaks are done by '|' character
    # The expressions %i, %h, %d, and %% are substituted for user/group name,
    # host name, domain name, and '%' respectively. For backward compatibility
    # %s behaves as %i but is deprecated.
    MESSAGE         = Hello user %i, I've noticed you use too much space\
    on my disk in %h.%d.|Delete your files on the following filesystems:|
    # Text in the end of the mail (if not specified, default text using SUPPORT and PHONE
    # is created)
    SIGNATURE       = See you!|                     Your admin of %h|
    # Following text is used for mails about group exceeding quotas
    GROUP_MESSAGE   = Hello, a group '%i' you're member of use too much space at %h.

使用\ ``cron``\ 定期检查的，系统即可以定期检查用户的磁盘使用情况，并及时提醒用户。

监控报表
=========
作为SA(system administrator)你需要关注用户的磁盘配额使用量。

命令\ ``quota``\ 和\ ``repquota``\ 可以帮你完成这些任务。

查看配额设定
-------------
``quota``\ 可以用于查看对某个用户或工作组的磁盘配额设定情况。如：查看sheldon的\
配额设定值。

.. sourcecode:: bash

    quota -su sheldon
    # 选项-s使得输出值便于阅读
    # 命令输出值：
    Disk quotas for user sheldon (uid 1000):
        Filesystem   space   quota   limit   grace   files   quota   limit   grace
    /dev/mapper/vg-scale
                    30720K* 10240K  30720K   6days       2       0       0

查看某个工作组的配额设定，使用选项\ ``-g``\ 即可。

.. sourcecode:: bash

    quota -sg bigbang
    # 命令行输出值：
    Disk quotas for group liuhui (gid 1000):
        Filesystem   space   quota   limit   grace   files   quota   limit   grace
    /dev/mapper/vg-scale
                    30720K    100M    200M               2       0       0

查看详细使用情况
----------------
``repquota``\ 命令用于查看某个文件系统上所有用户，工作组的磁盘配额使用情况。例如：
*   查看用户报告使用选项\ ``-u``\ :

    .. sourcecode:: bash

        repquota -su .
        *** Report for user quotas on device /dev/mapper/vg-scale
        Block grace time: 7days; Inode grace time: 7days
                                Space limits                File limits
        User            used    soft    hard  grace    used  soft  hard  grace
        ----------------------------------------------------------------------
        root      --     13K      0K      0K              2     0     0
        sheldon   +-  30720K  10240K  30720K  6days       2     0     0

*   查看工作组报告选项\ ``-g``\ :

参考资料
=========
