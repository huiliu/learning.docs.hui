入侵检测与响应
***************

入侵分析员的真正价值体现在其估定现象和情况的能力\ [#ref1]_\ 。

入侵检测工具只是用来辅助的，关键还是要靠人来驱动。

可能遭到入侵的症状
===================

:rfc:`2196` "Site Security Handbook"和\ `CERT`_\ 的“Steps for Recovering from a
UNIX or NT System Compromise”提供了一些检查异常状态列表。

日志中的异常
---------------
* **系统日志文件**\t 系统日志中原因不明的条目、日志文件缩减、日志文件丢失等
* **系统守护进程报告**
* **异常控制台和终端消息**\t 莫名其妙的终端信息
* **反复的访问尝试**\t  通过FTP或Web反复进行登录尝试，访问非法文件等操作

系统配置异常
--------------
配置文件和系统脚本被修改，未调用的进程在运行、非预期端口的使用、网络设备状态改\
变等。

* **cron任务**\ 检查cron配置和可执行程序是否正常
* **ps命令显示不能解释的服务和进程**\t  注意\ ``ps``\ 命令也可能被替换
* **netstat, lsof, tcpdump等检测到非预期的连接和端口使用**
* **系统崩溃及丢失进程**\t  系统和服务非预期的崩溃都是可疑的
* **设备配置改变**\t    网络设备被设定为混杂模式等

文件系统中的异常
-----------------
* **出现新的文件和目录**
* **setuid程序**
* **文件丢失**
* **文件系统大小迅速改变**\t    磁盘被快速耗尽
* **公共文件被修改**\t  FTP和Web内容被修改
* **/dev目录中出现新文件**\t    /dev目录下出现新的ASCII文件或目录，有可能为木马\
  的配置文件

帐号异常
----------
* **出现新的帐号或原帐号被修改**\t  */etc/passwd*\ 中出现新帐号，非预期的UID进\
  程；突然无法登陆等
* **用户登陆记录**\t    ``last``\ 显示非预期的登陆，登陆相关日志被编辑过(\
  /var/log/lastlog,  /var/log/pacct等)
* **根用户被修改**\t    特别注意用户的相关配置文件，环境变量是否被修改


系统性能方面
-------------
* **异常的高负载**
* **频繁的磁盘访问**

入侵后采取的措施
==================
* **断开网络**
* 检查系统日志，配置文件，suid程序，与备份进行对比
* 检查运行中的进程和打开的端口
* 观察系统中任何不断变化的信息
* 从干净的或备份系统启动进入，对磁盘进行检查
* 判断黑客如何成功进入系统的，对系统做了哪些手脚
* 如果可能全新安装系统
* 纠正系统弱点
* 安装和配置一个系统完整性检查软件包
* 启动全部日志功能
* 恢复已知、未受感染用户文件和特定的配置文件
* 为系统的二进行文件和静态文件创建MD5校验和
* 更新系统补丁
* 对新安装的二进制程序创建MD5校验和，并将其存于其它系统中
* 监控系统，观察黑客是否再次进行非法访问尝试

事件报告
=========


参考资料
==========

.. [#ref1] Linux防火墙（第三版） Steve Suehring, Robert L. Ziegler著， 何泾沙等译，机械工业出版社，2006

.. _CERT:   http://www.cert.org/tech_tips/win-UNIX-system_compromise.html
