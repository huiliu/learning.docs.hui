入侵检测工具－Snort
*********************



.. author:: default
.. categories:: devops
.. tags:: devops, deploy
.. comments::
.. more::



`barnyard2`\ 可以将\ `snort`\ 的日志输出到MySQL中，便于查询分析

安装说明
=========

常见故障
========
在安装使用过程中可能遇到的问题：

1.  启动\ `barnyard2`\ 时，可能出现“\
    **ERROR: Unable to open directory '' (No such file or directory)
    ERROR: Unable to find the next spool file!**\ ”

    可能原因是启动脚本的命令行默认启动参数不合适。(FreeBSD中默认为: \
    **-c configfile -D**)，从命令行用\
    `barnyard2 -D -c configfile -d /var/log/snort -f snort.log`\ 可以正常启动。

2.  同样是启动barnyard2，出现内存不足：\
    **FATAL ERROR: Unable to allocate memory!  (3414356543 requested)**\ 。
    
    增加内存吧

3.  在Gentoo上启动时会发生错误：\ “\ **FATAL ERROR: Can't find pcap DAQ!**\ ”。

    原因是因为\ `snort`\ 找到\ ``daq``\ 库文件，使用命令行参数\
    ``--daq-dir libdaq``\ 。或者在配置文件\ ``/etc/snort/snort.conf``\ 中设定：

    ::

        # config daq: pcap
        config daq_dir: /usr/lib64/daq
        # config daq_mode: inline
        # config daq_var: <var>

4.  在Gentoo上由\ ``systemd``\ 启动\ ``snort``\ 时会发生错误：
    “\ **FATAL ERROR: Can't set DAQ BPF filter to '**\ ”

    原因还没有找到，从命令行启动没有过任务问题。


参考资料
==========
