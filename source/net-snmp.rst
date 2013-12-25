Net-SNMP使用心得
*********************

权限控制
=========

传统的访问控制
---------------

* rouser [-s SECMODEL]  USER [noauth|auth|priv [OID | -V VIEW [CONTEXT]]]
  rwuser [-s SECMODEL]  USER [noauth|auth|priv [OID | -V VIEW [CONTEXT]]]

* rocommunity COMMUNITY [SOURCE [OID | -V VIEW [CONTEXT]]]
  rwcommunity COMMUNITY [SOURCE [OID | -V VIEW [CONTEXT]]]

基于视图的访问控制
-------------------

* com2sec  [-Cn CONTEXT] SECNAME SOURCE COMMUNITY
  com2sec6  [-Cn CONTEXT] SECNAME SOURCE COMMUNITY

* com2secunix [-Cn CONTEXT] SECNAME SOCKPATH COMMUNITY 


* group GROUP {v1|v2c|usm|tsm|ksm} SECNAME


* view VNAME TYPE OID [MASK]

* access GROUP CONTEXT {any|v1|v2c|usm|tsm|ksm} LEVEL  PREFX  READ  WRITE NOTIFY


类型视图访问控制
-------------------


Tips
=====

删除不必要的项
------------------

使用SNMP获取磁盘IO数据时，会读取到大量ram, loop, fd等分区的数据，会增加网络传输\
量，也会使后面的处理稍微复杂一点。如果能够直接这些多余的信息去除最好。好在已经\
有人放出了补丁可以完成此任务\ [#ref1]_\ 。在最新的5.7.2中已经加入了此功能，\
CentOS 6.4的Net-SNMP 5.5中还没有此功能。

Net-SNMP 5.7.2中引入了新的参数：\ ``diskio_exclude_loop, diskio_exclude_ram,\
diskio_exclude_fd``\ 将它们设定为"yes"，就可以去除磁盘IO表\ ``.1.3.6.1.4.1.2021\
.13.15.1.1.2``\ 中"*ram, loop*"等。

.. sourcecode:: bash

   diskio_exclude_fd    yes
   diskio_exclude_ram   yes
   diskio_exclude_loop  yes

.. [#ref1] http://sourceforge.net/p/net-snmp/patches/1129/

问题集锦
=========
1.  查询某些项目时提示“\ ``No more variables left in this MIB View (It is past\
    the end of the MIB tree)``\ ”

    原因： 极有可以是VACM控制中的视图控制限制OID范围
