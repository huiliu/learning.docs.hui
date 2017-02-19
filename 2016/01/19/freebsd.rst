FreeBSD点滴
**************

.. author:: default
.. categories:: tips
.. tags:: devops, tips, bsd
.. comments::
.. more::


FreeBSD默认使用的是\ ``csh``\ 刚从Linux切换过去感觉相当的不爽，可以通过命令\
``chsh``\ 来改变帐户shell，比较生猛的方法：直接修改"*/etc/passwd*"文件。

本地化问题 - Localization
==========================
默认情况下显示中文会是乱码，从Linux切换过去，所以知道需要修改环境变量\ ``LC_*``\
和\ ``LANG``\ 的值。FreeBSD在这方面与Linux不太一样。

FreeBSD中可以在用户配置文件\ ``~/.login_conf, ~/.profile, ~/.bashrc, ~/.cshrc``\
中设定环境变量\ ``LANG``\ 和\ ``MM_CHARSET``\ 来进行调整。

调整Locale设定的两种方法
---------------------------
登陆级别的设定
^^^^^^^^^^^^^^^
1.  系统级设定\ ``/etc/login.conf``
    修改\ ``/etc/login.conf``\ 中的设定，将对所有用户生效。如：

    .. code-block:: text

        language_name|account_type_description:\
            :charset=MIME_charset:\
            :lang=locale_name:\
            :tc=default:

    修改\ ``/etc/login.conf``\ 后，需要运行\ ``cap_mkdb``\ 来更新数据库：

    .. code-block:: bash

        cap_mkdb /etc/login.conf 

2.  用户级设定\ ``~/.login.conf``

    与系统级设定语法相同，每个用户可以自定义相应的环境参数。

    .. code-block:: text

        me:\
            :charset=utf-8:\
            :lang=en_US.UTF-8:

从上面可以看出BSD的设定要求与Linux还是颇为相似的，只是BSD的格式与Linux下的\ ``\
/etc/login.conf``\ 不一样，而且Linux下的很多设置都写在\ ``/etc/profile``\ 中。\
关于\ ``login.conf``\ 的详细设定可以查看man手册。

另外还有一些常用命令可以用来调整Locale设定：

*   ``vipw``\ (root)将会打开一个类似\ ``/etc/passwd``\ 的文件，对于普通用户，\
    ``vipw``\ 打开的文件比\ ``/etc/passwd``\ 多三个分隔符“:”，其中有一个位置是\
    用来设定Language。如下：

    .. code-block:: text

        # /etc/passwd
        xxxxxx:\*:1002:1002:xxxxxx:/home/xxxxxx:/usr/local/bin/bash

        # vipw打开的文件
        xxxxxx:passwd:1002:1002:language:0:0:xxxxxx:/home/xxxxxx:/usr/local/bin/bash

*   ``adduser``\ 添加用户时进行设定。

    *   修改添加用户时的配置文件\ ``/etc/adduser.conf``\ ，再其中设定添加用户时\
        使用的Language－\ ``defaultclass = language``\ ，\ ``/etc/adduser.conf``
        的详细配置查看Man手册。

        .. code-block:: ini

            defaultclass = zh_CN.UTF-8

    *   命令提示时输入：

        .. code-block:: bash

            adduser
            ... ...
            Enter login class: default[]:

    *   由命令行参数指定：\ ``adduser -class language``

*   ``pw`` 当\ ``pw``\ 用于创建新用户时，可以通过命令行参数来指定Language。如：

    .. code-block:: bash

        pw useradd userName -L language

设定Shell启动文件
^^^^^^^^^^^^^^^^^^^
1.  文件\ ``/etc/profile``

    .. code-block:: bash

        export  LANG=en_US.UTF-8
        export  MM_CHARSET=UTF-8

2.  文件\ ``/etc/csh.login``

    .. code-block:: csh

        setenv  LANG        en_US.UTF-8
        setenv  MM_CHARSET  UTF-8

3.  添加用户，HOME

Shell命令与Linux的不同
=======================

sed
-----
在Linux下，命令\ `sed -i 's/a/b/g' file`\ 可以直接替换文件中的内容，不过在\
FreeBSD上却需要在选项\ ``-i``\ 后面加上一个空字符""才可以完成同样的任务。看看\
``man``\ 手册中是怎么说明的。

*   Linux中关于选项\ ``-i``\ 的说明：

    -i[SUFFIX], --in-place[=SUFFIX]

        edit files in place (makes backup if extension supplied)

*   FreeBSD中关于选项\ ``-i``\ 的说明：

    -i extension

        Edit files in-place similarly to -I, but treat each file independently
        from other files.  In particular, line numbers in each
        file start at 1, the “$” address matches the last line of the
        current file, and address ranges are limited to the current file.
        (See Sed Addresses.)  The net result is as though each file were
        edited by a separate sed instance.

