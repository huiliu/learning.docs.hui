Samba
*******



.. author:: default
.. categories:: devops
.. tags:: devops, deploy
.. comments::
.. more::


用Linux连接Samba服务器：
{{Cmd|``smbclient`` //192.168.6.3/ ``-U`` test}}

配置文件
=========
关于samba的配置文件，请查看man手册\ [#man]_

配置日志
---------
``Samba``\ 有自己的一套日志机制\ [#ref]_\ ，既可自己(Samba)定义存储日志文件，也\
可以将日志交由系统日志系统处理，也可以选择同时发送给两者。在配置文件
``/etc/samba/smb.conf``\ 中与日志相关的配置选项有："``log level``",\
"``debuglevel``","``syslog``"，"``syslog only``"等。

日志级别可以设定为0-10的一个整数，值越大，日志越详细。官方文档中推荐3及以上的级\
别只适用于开发人员，对一般系统管理员没有必要。使用日志级别 0 将避开除一些启动消\
息和关键错误之外的任何消息。

Samba还提供了一系列更加详细的参数，可以更好的控制日志信息的输出。

.. sourcecode:: text

    all。该参数是可选的：如果您不指定额外的关键字，假定使用 all。
    tdb。与不重要的数据库相关的日志消息，这是 Samba 使用的键值存储。
    printdrivers。打印机驱动程序管理例程。
    lanman。 NT LAN Manager 调试。
    smb。 SMB 协议调试。
    rpc_parse。远程过程调用（RPCs）解析。
    rpc_srv。服务器端 RPCs。
    rpc_cli。客户端 RPCs。
    passdb。在 Samba 主机上存储密码的旧有方式。
    sam。本地 Samba 帐户数据库。
    auth。Samba 内与用户身份验证相关的各个模块。
    winbind。用于允许Microsoft用户透明地登录到UNIX系统的组件。
    vfs。为 Virtual File System 模块调试消息，允许您通过可插入模块添加功能到Samba。
    idmap。在UNIX 用户IDs与Microsoft安全标识符之间映射身份。
    quota。与配额处理相关的消息，同时由Microsoft Windows NT策略和UNIX文件系统处理。
    acls。访问控制列表处理。
    locking。文件锁定状态和错误。
    msdfs。与 Samba 的分布式文件系统支持相关的日志消息。
    dmapi。数据管理应用程序编程接口（API）功能。必须使用第三方DMAPI实现编译Samba来使用该功能。
    registry。Windows 注册表的模仿。

要使用这些额外的日志记录，将\ ``关键字``\ 和\ ``值``\ 附加到日志级别参数，用冒\
号（:）分割。如：

.. sourcecode:: ini

    log level = 1 auth:3

系统默认的日志文件定义为：

.. sourcecode:: ini

    # "``%m``"为宏
    log file = /var/log/samba/log.%m

如此，samba的日志文件将被分割为一些小部分，

安全考量
--------
``Samba``\ 中提供了很多与安全控制相关的参数。

只允许特定IP范围的用户访问
^^^^^^^^^^^^^^^^^^^^^^^^^^^
*   ``hosts allow``\ 可以实现对服务访问进行控制，限制访问客户的来源，可以指定主\
    机名，IP地址，网段等；此指令可以用于\ ``[global]``\ ，也可以用于单个共享项\
    目。如：

    .. sourcecode:: ini

        # 配置文件/etc/samba/smb.conf
        # allow all IPs in 150.203.*.*; except one
        hosts allow = 150.203. EXCEPT 150.203.6.66
        
        # 允许某个网段的所有主机访问
        hosts allow = 192.168.1.0/255.255.255.0

        # allow a couple of hosts
        hosts allow = lapland, arvidsjaur

        # allow only hosts in NIS netgroup "foonet", but deny access from one particular host
        hosts allow = @foonet
        hosts deny = pirate

    .. note::
    
        既可以对某一个共享目录进行限制，可以对全局进行限制
    
    另外，当然也可以使用系统防火墙\ ``iptables``\ 来过滤掉一些不希望的用户。
    
    .. todo::
    
        使用iptables保护samba服务器

*   ``hosts deny``\ 的用途与\ ``hosts allow``\ 刚好相反，用于拒绝用户访问。
*   一种比较好的访问控制策略是：\ **拒绝所有的访问，显式的说明允许那些主机访问**\ 。

    .. sourcecode:: ini

        # /etc/samba/smb.conf, hosts allow使用示例
        [global]
         # deny all
         hosts deny = 0.0.0.0/0
        
        [common]
         # 允许某个网段的所有主机访问
         hosts allow = 192.168.1.0/255.255.255.0

限制特定用户访问
^^^^^^^^^^^^^^^^^
*   选项"``invalid users``"列出不允许访问当前共享资源的用户。This is really a
    paranoid check to absolutely ensure an improper setting does not breach your
    security. 用于保证共享资源的绝对安全。
*   选项"``valid users``"可以用于限制共享资源只对指定的用户开放。

对于选项"``invalid users``"和"``valid users``"的值，有以下规则：

*   以"``@``"开头的字符串将首先被解释为NIS网络组，如果在NIS网络组中不存在，被解\
    释为UNIX组
*   以"``+``"开头的字符串将``仅``被解释为UNIX组
*   以"``&``"开头的字符串将``仅``被解释为NIS网络组
*   "``+``"和"``&``"可以组合使用，即"+&group"或"&+group"，首先check前面那个

例如：

.. sourcecode:: ini

    valid users = sheldon, penny, @wheel

匿名共享
^^^^^^^^^^^^
*   匿名共享有多种方式进行：
    
    *   最简单的是全局匿名共享，将[global]中的\ ``security``\ 设置为\ ``share``\
        即可，这种方式一般用于打印机共享；
    *   另外一种是对指定的资源进行匿名共享，而一部分还是需要密码访问。

*   全局共享模式：
    
    *   将[global]中\ ``security=share``
    *   在相应的资源中添加\ ``guest ok = yes``, ``public = yes``

*   部分匿名共享模式

    *   在[global]中，将\ ``security=user``
    *   在[global]中，将\ ``map to guest = Bad User``
    *   在相应的共享资源中添加：\ ``guest accout = nobody``, ``guest ok = yes``

详细说明请查看man smb.conf(5)的\ **Security**


    如果将选项"``guest ok``"和"``public``"（两者等同）设为"``ok``"，则相应的共\
    享资源无需密码即可访问。相应的访问权限取决于选项"``guest account``"的设定值。

    选项"``guest account``"的设定值为一个用户名，表示samba客户端以guest身份访问\
    共享资源时，在samba服务器上所使用的用户身份。
    选项"``guest only``"设定为"``yes``"，共享资源将只允许匿名访问。"``guest
    ok``"为"``yes``"时，此选项才能生效，如：

    .. sourcecode:: ini

        guest ok = <yes|no>
        public = yes

读写控制
^^^^^^^^^
*   只读相关

    *   选项"``read only``"

    *   选项"``read list``"指明哪些用户对共享资源具有只读权限，无论"
        ``read only``"是否为\ *yes*\ ，当\ ``security = share``\ 时，此选项无效

*   读写相关

    *   选项"``writeable``"

    *   选项"``write list``"指明哪些用户对共享资源具有读写权限，无论"``read
        only``"是否为\ *yes*\ ，当\ ``security = share``\ 时，此选项无效

"``read list``"和"``write list``"的语法与''invalid users''相同。

.. sourcecode:: ini

    read only = yes
    read list = sheldon, @wheel
    
    writable = yes
    write list = cooper, @root

符号连接的问题
^^^^^^^^^^^^^^^^
默认禁止打开连接到共享区域之外的符号链接，这也是安全的做法。

相关选项有"``allow insecure wide links``", "``wide links``"，"``follow
symlinks``"，但我并没有成功连接到共享区域之外，可以还有地方没有设定正确。

Windows, Linux混合使用问题
^^^^^^^^^^^^^^^^^^^^^^^^^^^


文件名大小写
^^^^^^^^^^^^
因为Windows系统不区分大小写，而Linux(Unix)系统是区分大小。为了消除可能带来的问\
题，Samba中也有相应的选项设定。"``case sensitive``"，"``default case``"，
"``preserve case``"

编码问题
^^^^^^^^^^

隐藏文件问题
^^^^^^^^^^^^^^
*   "``hide dot files = yes``"将隐藏dot文件。Windows中打开文件夹选项中的查看系\
    统隐藏文件即可查看dot文件

*   选项"``hide files``"的内容为一个文件和目录列表。其中的文件或目录被隐藏但是\
    可以被访问。这个选项会影响到samba服务器的性能，因为当导出共享资源时，samba\
    服务器将强制检查每个文件和目录。值的字符串可以使用通配符，详细请查看\
    ``man smb.conf``

*   "``veto files``"指定某些文件或目录既不可见也不可访问。当某个文件或目录因为\
    递归被删除时，会引发删除失败，除非选项"``delete veto files``"设定为“''yes''”

帐户问题
==========
通常，访问Linux(Unix)服务器上的Samba服务需要用户在服务器上拥有一个帐户。当\
Windows用户（在Linux服务器上没有帐户的用户）首次访问时，需要创建一个帐户才可以\
访问。此时，{{Highlight|smb.conf}}中有一系列相关的选项可以完成这些任务："``add
user script``"，"``add user to group script ``"，"``add group script``"，"``add
machine script``"，"``delete user script``"等等，同时``security``不能设定为
"``share``"。

当Windows用户首次登陆Samba服务器时，samba向password server请求授权，如果授权通\
过但是Linux服务器上没有相应的用户，samba就会以root权限运行上面选项列出的命令或\
脚本创建相应用户。这样Linux Samba服务器就可以动态的为windows用户创建共享帐号。

更好的方法应该是使用LDAP认证机制。

创建samba用户
--------------

samba用户必须时系统中的用户，即其用户名必须在\ */etc/passpwd*\ 中，但是仅此是不\
够的，samba服务器使用独立的授权数据库，即：samba用户的密码与访问系统的密码没有\
关系。如果想让一个系统用户可以访问samba共享资源（非share资源），需要将其加入到\
samba中：

.. sourcecode:: bash

    smbpasswd -a username
    # New SMB password:
    # Retype new SMB password:
    # Added user username.

验证用户是否成功加入：

.. sourcecode:: bash

    pdbedit -w -L

客户端
========
*   阅读IBM Developworks上关于samba的文章

Linux下浏览，挂载Samba共享文件
------------------------------
*   使用"``smbclient``"可以像浏览FTP一样浏览Samba服务器的共享资源。具体命令如下：
    ``smbclient //ShareName_OR_IP/ShareDir [-U username]``

    .. note::

        特别需要注意是"/"而不是"\\"，并且一定要跟随共享目录名。

*   挂载Samba共享资源到本地目录下，像访问NFS，本地硬盘一样访问。

    首先确认是否安装"``cifs-utils``"，"``cifs-utils``"中提供了挂载samba共享资源\
    的命令:\ ``mount.cifs``\ 。

    .. sourcecode:: bash

        yum install cifs-utils

        # 运行mount.cifs挂载：
        mount.cifs //192.168.6.3/test ./tmp -o user=test

    .. note::

        如果挂载好目录后发现无法读写samba共享目录，可能是因为samba服务器上的\
        SELinux阻止了相应的操作。解决方法：

        *   查看与samba有关的SELinux设定值。
            ``getsebool -a | egrep '(samba)|(smb)|(nmb)|(win)'``

        *   调整SELinux参数：
            ``setsebool -P samba_export_all_ro=1 samba_export_all_rw=1``

        *   另外可能要调整samba共享目录的SELinux标签：
            ``chcon -Rt samba_share_t /common``


开机自动挂载
-------------
对于需要密码才能访问的Samba共享，挂载时会提示要求输入密码，当然不能将用户名和密\
码作为参数写在\ */etc/fstab*\ 中。此时需要另外一个挂载参数\ ``credentials``\ 来\
*指定一个密码授权文件*\ 。如：

.. sourcecode:: text

    # /etc/fstab 开机自动挂载samba共享内容
    //192.168.6.3/user1 /mnt/user1  cifs    credentials=/etc/samba/smbcred  0 0

其中文件"*/etc/samba/smbcred*"为授权文件，里面存储着samba用户名和密码：

.. sourcecode:: text

    # /etc/samba/smbcred 自定义授权文件
    username=user1
    password=password

.. warning::

    注意"*/etc/samba/smbcred*"的权限应该为600

常见问题
==========
1.  登陆时提示\ ``session setup failed: NT_STATUS_LOGON_FAILURE``

    提示帐户不存在。

    **原因：**\ 登陆使用的用户在系统中存在，但是没有使用命令\ ``smbpasswd -a
    name``\ 将用户加入到samba中。

2.  使用命令\ ``smbclient //host/dir -U user``\ 登陆时出现错误\
    ``tree connect failed: NT_STATUS_BAD_NETWORK_NAME``\ 。

    **原因：**\ 连接的目录\ ``dir``\ 不正确，不存在。

3.  当使用匿名共享与认证共享混合时，Windows访问可能会提示"\ **不允许一个用户使\
    用一个以上用户名与一个服务器或共享资源的多重连接**\ "。

    **解决：**\ 打开cmd，输入命令 ``net use``\ 查看本机当前的共享连接，可以发现\
    本机当前已经连接上了samba服务器上的匿名共享资源，清除这个连接(\
    ``net use * /del``\)，再直接输入需认证访问的资源的路径即可。

4.  ``/var/log/messages``\ 中一直收到错误日志消息：\ ``failed to retrieve
    printer list: NT_STATUS_UNSUCCESSFUL``\ 。

    **解决：**\ 禁用samba的printer服务。在samba的配置文件\
    ``/etc/samba/smb.conf``\ 中添加：

    .. sourcecode:: ini

        load printers = no
        printing = bsd
        printcap name = /dev/null

参考资料
========
.. [#man]   `smb.conf(5) <http://www.samba.org/samba/docs/man/manpages-3/smb.conf.5.html>`_
.. [#ref]   `学习 Linux，302（混合环境）: 配置 Samba <http://www.ibm.com/developerworks/cn/linux/l-lpic3-312-1/>`_
