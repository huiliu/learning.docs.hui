docker使用小记
***************
`funtoo`_\ Wiki上有一篇非常好的文章介绍内核配置及\ ``lxc``\ 的配置。[#]_

.. _funtoo:  http://www.funtoo.org


错误说明
========
1.  运行命令\ ``docker run centos echo 'hello world'``\ 结果提示出错：

    .. sourcecode:: text

        lxc-start: Device or resource busy - failed to mount a new instance of
        '/dev/pts'
        lxc-start: failed to setup the new pts instance
        lxc-start: failed to setup the container
        lxc-start: invalid sequence number 1. expected 2
    
    根据错误提示应该是使用\ */dev/pts*\ 失败，检查一下\ ``lxc``\ 的配置文件，结\
    果发现\ */etc/lxc*\ 目录下为空，也就是说\ ``lxc``\ 使用的默认配置。再检查一\
    下发现有一个命令\ ``lxc-checkconfig``\ 可以检查\ ``lxc``\ 的配置，运行一下\
    得到：

    .. sourcecode:: text
        :emphasize-lines: 8, 16

        --- Namespaces ---
        Namespaces: enabled
        Utsname namespace: enabled
        Ipc namespace: enabled
        Pid namespace: enabled
        User namespace: enabled
        Network namespace: enabled
        Multiple /dev/pts instances: missing

        --- Control groups ---
        Cgroup: enabled
        Cgroup clone_children flag: enabled
        Cgroup device: enabled
        Cgroup sched: enabled
        Cgroup cpu account: enabled
        Cgroup memory controller: missing
        Cgroup cpuset: enabled

        --- Misc ---
        Veth pair device: enabled
        Macvlan: enabled
        Vlan: enabled
        File capabilities: enabled

        Note : Before booting a new kernel, you can check its configuration
        usage : CONFIG=/path/to/config /usr/sbin/lxc-checkconfig

    从配置文件检查可以发现\ **Multiple /dev/pts**\ 和\ **Cgroup memory
    controller**\ 两功能不支持，经google，应该是内核配置的问题。[#]_\ 重新配置\
    相应内核选项，并编译即可。

    .. sourcecode:: text
        :emphasize-lines: 1

        │ Symbol: DEVPTS_MULTIPLE_INSTANCES [=y]
        │ Type  : boolean
        │ Prompt: Support multiple instances of devpts
        │   Location:
        │     -> Device Drivers
        │       -> Character devices
        │         -> Enable TTY (TTY [=y])
        │ (1)       -> Unix98 PTY support (UNIX98_PTYS [=y])
        │   Defined at drivers/tty/Kconfig:123
        │   Depends on: TTY [=y] && UNIX98_PTYS [=y]

参考资料
========
.. [#]  http://www.funtoo.org/Linux_Containers
.. [#]  https://bugs.launchpad.net/ubuntu/+source/linux-ti-omap4/+bug/787749
