如何让特定用户通过SSH登陆后自动chroot？
***************************************

原文链接：http://how-to.linuxcareer.com/how-to-automatically-chroot-jail-selected-ssh-user-logins

简介
======
本文将介绍如何使某个工作组中的用户通过SSH登陆系统后自动chroot。如果你想为系\
统中的用户提供一个受限的系统环境，并且与主系统相隔离，那么这个方法非常有用\
。你也可以用这个方法建立一个简单的SSH蜜罐；通过本文你将学到：

1.  如何建立一个基本的\ ``chroot``\ 环境
2.  如何配置SSH服务使得特定用户登陆后自动进行\ ``chroot``\ 环境

建立基本的chroot环境
=====================
首先我们建立一个简单的\ ``chroot``\ 环境，其中只包含\ ``bash``\ 一个可执行\
程序。为了达到这一目标，先创建一个chroot目录：

.. code-block:: bash

    mkdir   /home/jail

接下来我们需要将应用“\ **/bin/bash**\ ”和其依赖的动态链接库拷贝至\
``/home/jail``\ 下。使用命令\ ``ldd``\ 可以查看\ ``bash``\ 依赖那些动态库。

.. code-block:: bash

    ldd /bin/bash 
        linux-gate.so.1 =>  (0x001e7000)
        libtinfo.so.5 => /lib/libtinfo.so.5 (0x00820000)
        libdl.so.2 => /lib/libdl.so.2 (0x0034b000)
        libc.so.6 => /lib/libc.so.6 (0x0052b000)
        /lib/ld-linux.so.2 (0x00af0000)

我们手动创建必须的目录，并将\ ``/bin/bash``\ 及其依赖库拷贝到\
``/home/jail``\ 下相应的位置。

.. code-block:: bash

    cd /var/chroot/
    mkdir bin/ lib64/ lib/
    cp /lib/x86_64-linux-gnu/libtinfo.so.5 lib/
    cp /lib/x86_64-linux-gnu/libdl.so.2 lib/
    cp /lib/x86_64-linux-gnu/libc.so.6 lib/
    cp /lib64/ld-linux-x86-64.so.2 lib64/
    cp /bin/bash bin/

此时，一个最简单的chroot环境准备好了，我们可以看看chroot的效果：

.. code-block:: bash

    chroot /vat/chroot
    bash-4.2# ls /  
    bash: ls: command not found

从上面你可以看到\ ``bash``\ (shell)已经准备就绪，但是什么也不能做，甚至连\
``ls``\ 命令都没有。不同于上面手动拷贝所有命令和依赖文件，下面提供了一个简\
单的shell脚本来完成：

.. code-block:: bash

    #!/bin/bash
    # This script can be used to create simple chroot environment
    # Written by LinuxCareer.com <http://linuxcareer.com/>
    # (c) 2013 LinuxCareer under GNU GPL v3.0+
    
    #!/bin/bash
    
    CHROOT='/home/jail'
    mkdir $CHROOT
    
    for i in $( ldd $* | grep -v dynamic | cut -d " " -f 3 | sed 's/://' | sort | uniq )
      do
        cp --parents $i $CHROOT
      done
    
    # ARCH amd64
    if [ -f /lib64/ld-linux-x86-64.so.2 ]; then
       cp --parents /lib64/ld-linux-x86-64.so.2 /$CHROOT
    fi
    
    # ARCH i386
    if [ -f  /lib/ld-linux.so.2 ]; then
       cp --parents /lib/ld-linux.so.2 /$CHROOT
    fi
    
    echo "Chroot jail is ready. To access it execute: chroot $CHROOT"

上面的脚本中通过变量\ ``$CHROOT``\ 来设定chroot目录的路径，当前为：\
``/home/jail``\ 。你可以根据需要自行设定。准备好之后，使得脚本具有可执行权\
限，以你所期望添加的程序和文件名作为命令行参数运行脚本。例如：你需要运行命\
令\ ``ls, cat, echo, rm, bash, vi``\ ，使用命令\ ``which``\ 找到它们的完整\
路径，并作为脚本chroot.sh的参数运行。

.. code-block:: bash

    ./chroot.sh /bin/{ls,cat,echo,rm,bash} /usr/bin/vi /etc/hosts
    Chroot jail is ready. To access it execute: chroot /var/chroot

新的chroot环境准备好了，进行看看：

.. code-block:: bash

    # chroot /var/chroot
    bash-4.2# echo linuxcareer.com > file
    bash-4.2# cat file
    linuxcareer.com
    bash-4.2# rm file
    bash-4.2# vi --version
    VIM - Vi IMproved 7.3 (2010 Aug 15, compiled May  4 2012 04:25:35)

创建一个chroot用户组
=====================
建立一个用户组\ ``chrootjail``\ ，属于该组的用户通过ssh登陆系统时将被自动\
chroot。\ ::

    sudo groupadd chrootjail

添加用户到\ ``chrootjail``\ 组中：\ ::

    sudo adduser tester chrootjail
    Adding user `tester' to group `chrootjail' ...
    Adding user tester to group chrootjail
    Done.

配置sshd
=========
为了让\ ``chrootjail``\ 用户组中的用户通过ssh登陆时自动chroot到目录\
``/home/jail``\ 中，需要在sshd的配置文件\ ``/etc/ssh/sshd_config``\ 中添加\
配置：

.. code-block:: text

    Match group chrootjail
        ChrootDirectory /var/chroot/

重启sshd服务：

.. code-block:: bash

    sudo service ssh restart

通过ssh登陆测试
===================
.. code-block:: bash

    ssh tester@localhost
    tester@localhost's password: 
    bash-4.2$ ls
    bin  lib  lib64  usr
    bash-4.2$

结束语
==========
通过上面的说明你可以发现建立一个ssh的chroot环境是非常简单的。当用户登陆到\
chroot环境中，其home目录不存在时，它的当前工作目录为“\ ``/``\ ”。你可以根据\
需要对chroot环境进行更多的定制，如建立用户HOME目录，定义bash环境变量等。
