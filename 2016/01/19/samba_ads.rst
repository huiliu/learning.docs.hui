samba集成Active Directory
*************************



.. author:: default
.. categories:: devops
.. tags:: devops, deploy
.. comments::
.. more::



如何将Linux主机加入到Windows的域中呢？即Linux可以通过Windows域来登陆。

前提说明
=========
软件需求
--------
*   操作系统：CentOS 5.8
*   软件包： \ ``samba3x-winbind, samba3x-common, samba3x, samba3x-client``

系统环境
--------
*   Windows域：\ ``DEVEL.EXAMPLE.COM``
*   域控制器为：\ ``domain.devel.example.com``

确认samba支持LDAP和kerberos
----------------------------
执行命令\ ``smbd -b | grep KRB``\ 和\ ``smbd -b | grep LDAP``\ 观察是否有类似下\
面的信息输出。

.. sourcecode:: bash

    $ smbd -b | grep KRB
        HAVE_KRB5_H
        HAVE_KRB5_LOCATE_PLUGIN_H
        HAVE_ADDRTYPE_IN_KRB5_ADDRESS
        HAVE_DECL_KRB5_AUTH_CON_SET_REQ_CKSUMTYPE
        HAVE_DECL_KRB5_GET_CREDENTIALS_FOR_USER
        HAVE_INITIALIZE_KRB5_ERROR_TABLE
        HAVE_KRB5
        HAVE_KRB5_AUTH_CON_SETUSERUSERKEY
        HAVE_KRB5_AUTH_CON_SET_REQ_CKSUMTYPE
        HAVE_KRB5_C_ENCTYPE_COMPARE
        HAVE_KRB5_C_VERIFY_CHECKSUM
        HAVE_KRB5_DEPRECATED_WITH_IDENTIFIER
        HAVE_KRB5_ENCRYPT_BLOCK
        HAVE_KRB5_ENCRYPT_DATA
        HAVE_KRB5_ENCTYPE_TO_STRING
        HAVE_KRB5_ENCTYPE_TO_STRING_WITH_SIZE_T_ARG

    $ smbd -b | grep LDAP
        HAVE_LDAP_H
        HAVE_LDAP
        HAVE_LDAP_ADD_RESULT_ENTRY
        HAVE_LDAP_INIT
        HAVE_LDAP_INITIALIZE
        HAVE_LDAP_SASL_WRAPPING
        HAVE_LDAP_SET_REBIND_PROC
        HAVE_LIBLDAP
        LDAP_SET_REBIND_PROC_ARGS

共享Active Directory帐户
=========================
samba和kerberos
----------------
samba可以通过\ ``kerberos``\ 与AD域服务器通讯，对域用户进行验证。配置samba，需\
要编辑配置文件\ ``/etc/krb5.conf``

.. sourcecode:: text

    [logging]
     default = FILE:/var/log/krb5libs.log
     kdc = FILE:/var/log/krb5kdc.log
     admin_server = FILE:/var/log/kadmind.log
    
    [libdefaults]
     default_realm = DEVEL.EXAMPLE.CN
     dns_lookup_realm = false
     dns_lookup_kdc = false
     ticket_lifetime = 24h
     forwardable = yes
    
    [realms]
     # 注意此处域一定要大写
     DEVEL.EXAMPLE.CN = {
      # kdc 用于指定Active Directory域控制器
      kdc = domain.devel.example.cn
      # admin_server
      admin_server = domain.devel.example.cn
      # default_domain  默认使用的域
      default_domain = devel.example.cn
     }
    
    [domain_realm]
     .devel.example.cn = DEVEL.EXAMPLE.CN
     devel.example.cn = DEVEL.EXAMPLE.CN
    
    [appdefaults]
     pam = {
       debug = false
       ticket_lifetime = 36000
       renew_lifetime = 36000
       forwardable = true
       krb4_convert = false
     }

使用命令\ ``kinit Administrator@DEVEL.EXAMPLE.CN``\ 来测试

winbind
--------
守护进程\ ``winbindd``\ 将与AD域进行通讯并为Linux提供帐户验证。\ ``winbindd``\
是通过PAM(Pluggable Authentication Modules)来实现此功能的。

确认存在模块\ ``/lib64/security/pam_winbind.so``\ ，如果是手动编译安装的请将\
``pam_winbind.so``\ 链接至前面的位置。

配置系统使用\ ``pam_winbind``\ 模块：在\ ``/etc/pam.d``\ 目录下建立文件\
``system-auth-winbind``\ ，并将其链接至\ ``system-auth``\ 。注意备份好原文件。

.. sourcecode:: text

    # /etc/pam.d/system-auth -> /etc/pam.d/system-auth-winbind
    # 关于PAM模块的配置请查看相关文档
    auth required pam_env.so
    auth sufficient pam_unix.so likeauth nullok
    # 使用winbind进行帐户验证
    auth sufficient pam_winbind.so use_first_pass
    auth required pam_deny.so
    
    account required pam_unix.so
    account sufficient pam_succeed_if.so uid < 100 quiet
    account sufficient pam_winbind.so use_first_pass
    account required pam_permit.so
    
    password requisite pam_cracklib.so retry=3 type=
    password sufficient pam_unix.so nullok use_authtok md5 shadow
    password sufficient pam_winbind.so use_first_pass
    password required pam_deny.so
    
    session required pam_limits.so
    session required pam_unix.so
    session required pam_winbind.so use_first_pass

另外模块\ ``pam_winbind``\ 的配置文件为：\ ``/etc/security/pam_winbind.conf``\
可以编辑：

.. sourcecode:: ini

    #
    # pam_winbind configuration file
    #
    # /etc/security/pam_winbind.conf
    #
    
    [global]
    
    # turn on debugging
    ;debug = yes
    
    # request a cached login if possible
    # (needs "winbind offline logon = yes" in smb.conf)
    ;cached_login = yes
    
    # authenticate using kerberos
    krb5_auth = yes
    
    # 如果home目录不存在则新建之
    mkhomedir = yes
    # when using kerberos, request a "FILE" krb5 credential cache type
    # (leave empty to just do krb5 authentication but not have a ticket
    # afterwards)
    ;krb5_ccache_type = FILE
    
    # make successful authentication dependend on membership of one SID
    # (can also take a name)
    ;require_membership_of =



Name Service Switch
--------------------
``Name Service Switch``\ 提供了一种标准机制，在该机制中，Linux计算机可以与常见服务进行交互，其中一个服务是身份验证。在使用这些服务时，Linux会查询 \ ``/etc/nsswitch.conf``\ 文件。请根据下列方法修改该文件，以便允许 Linux使用 Winbind 进行用户身份验证。

下面的代码突出了使用Winbind添加Winbind支持，以便允许用户参照AD DS Kerberos 5数据库，使用Winbind进行身份验证：

.. sourcecode:: text

    passwd: files winbind
    group:   files winbind


完成上面的配置后，启动守护进程\ ``winbindd``\ ，当前Linux服务器就可以与域服务器\
通讯了。 smb.conf ---------
为了让samba加入到AD域中工作，当然要配置\ ``smb.conf``\ ：

.. sourcecode:: ini

    [global]
        unix charset = utf8
        display charset = utf8
        dos charset = cp936
        load printers = no
        # AD域
        realm = devel.example.cn
        # ads or domain
        security = ads
        ; 域控制器服务器
        password server = domain.devel.example.cn
        ; 域
        workgroup = devel
        ; 映射AD域用户，组的uid,gid
        idmap uid = 10000-15000
        idmap gid = 10000-15000
        winbind enum users = yes
        winbind enum groups = yes
        ; 使用默认域
        winbind use default domain = yes
        winbind separator = \
        ; 设定AD域用户的默认shell，如果不进行设定，则为/bin/false
        template shell = /bin/bash
    
    [homes]
        comment = Home Directories
        read only = No
        browseable = yes

``net``\ 命令
=============
完成上面的配置操作后，可以通过\ ``net``\ 命令加入到AD域：

.. sourcecode:: bash

    # 根据提示输入密码。加入成功命令行会有提示
    sudo net ads join -U Administrator

    # 使用下面命令测试加入成功
    sudo net ads tesjoin

``net``\ 命令是一个非常强大完善的管理工具，其它详细信息请查看帮助\
``net help subcrommand``\

另外也可以通过winbindd提供的工具\ ``wbinfo``\ 来查询AD域的信息、资源等

.. sourcecode:: bash

    # 检查winbindd是否在运行
    wbinfo -p

    # 列出域中的用户
    wbinfo -u
    
    # 列出域中的组
    wbinfo -g
    
    wbinfo -t

参数资料
========
1.  `学习 Linux，302（混合环境）: 与 Active Directory 集成
    <http://www.ibm.com/developerworks/cn/linux/l-lpic3-314-3/index.html>`_

2.  `Samba & Active Directory <http://wiki.samba.org/index.php/Samba_%26_Active_Directory>`_

3.  http://wiki.samba.org/index.php/Samba,_Active_Directory_%26_LDAP

4.  http://www.samba.org/samba/docs/man/Samba-HOWTO-Collection/domain-member.html#ads-member
