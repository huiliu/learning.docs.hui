入侵检测工具－Snort
*********************

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


参考资料
==========
