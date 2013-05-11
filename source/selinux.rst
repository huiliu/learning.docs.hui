探索SELinux
***************

学习美国国家安全局的开源工具\ **SELinux**

问题
=====
昨天想把物理机上的WEB服务迁移到虚拟机上，一切准备就绪后，却发现BLOG连接上不上物\
理机上的数据库。

写了一小段代码在命令行测试php连接物理机上的数据库：

.. sourcecode:: php

    <?php
    // Usage without mysql_list_dbs()
    $link = mysql_connect('ctc225', 'wiki', 'wiki');
    $res = mysql_query("SHOW DATABASES");
    while ($row = mysql_fetch_assoc($res)) {
        echo $row['Database'] . "\n";
    }
    mysql_close($link);
    ?>

在命令行运行正常；移到WEB目录下，用浏览器打开，提示无法连接数据库。

我找啊找……

最后在日志"**/var/log/audit/audit.log**"中发现了些端倪：

| *type=SYSCALL msg=audit(1352534395.666:528): arch=c000003e syscall=42 success=no exit=-13 a0=c a1=7fffc08d7db0 a2=10 a3=c items=0 ppid=21873 pid=21877 auid=0 uid=48 gid=48 euid=48 suid=48 fsuid=48 egid=48 sgid=48 fsgid=48 tty=(none) ses=11 comm="httpd" exe="/usr/sbin/httpd" subj=unconfined_u:system_r:httpd_t:s0 key=(null)*

上面的日志说明\ *SELinux拦截了httpd进程连接远程数据库*\ 。问题原因找到了，解决\
办法呢？

解决方法
========
如果对SELinu熟悉应该很快就知道怎么解决了，可以偶不熟悉啊，又找啊找……

.. sourcecode:: bash

    getsebool -a | grep http

可以查看关于http服务的SELinux配置参数，发现：

    *httpd_can_network_connect_db => off*

从字面判断，将上面的变量设定为on应该就可以了

运行命令：

.. sourcecode:: bash

    setsebool httpd_can_network_connect_db=on

或者:

.. sourcecode:: bash

    togglesebool httpd_can_network_connect_db

就可以完成这个任务。 但是，系统重启之后，httpd\_can\_network\_connect\_db的值将\
再次变为"*off*"，即上面命令只是临时修改了SELinux的策略值。如果要修改默认值，需要\
在命令\ ``setsebool``\ 后面加上参数\ *-P*:[#ref1]_

.. sourcecode:: bash

    setsebool -P httpd_can_network_connect_db=on

总结
=====
出现问题先看看SELinux的日志，没有问题再去检查其它的，因为SELinux的问题相当隐秘，\
我都想了一天才想到的。

相关命令：
------------

.. sourcecode:: bash

    getsebool -a
    setsebool [-P] item=value
    togglesebool item
    semanage boolean -l

参考资料
========
.. [#ref1] http://publib.boulder.ibm.com/infocenter/lnxinfo/v3r0m0/index.jsp?topic=%2Fliaai%2Fselinux%2Fliaaiselinuxapache.htm
