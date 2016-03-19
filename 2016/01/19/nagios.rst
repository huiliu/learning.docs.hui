监控工具-Nagios
****************



.. author:: default
.. categories:: devops
.. tags:: devops, deploy
.. comments::
.. more::




问题
=====
1.  当某个服务出现故障，准备重新安装执行时，发生错误\
    ``Could not open command file '/usr/local/nagios/var/rw/nagios.cmd' for update!``

    查看\ ``/usr/local/nagios/var/rw/nagios.cmd``\ 的权限为：\ ::

        ll /usr/local/nagios/var/rw/nagios.cmd
        prw-rw----. 1 nagios nagios 0 Dec 22 20:01 nagios.cmd

    再查看Nagios的配置文件“\ ``/etc/local/nagios/etc/nagios.cfg``\ ”，可以发现:

    .. code-block :: text

        # EXTERNAL COMMAND FILE
        # This is the file that Nagios checks for external command requests.
        # It is also where the command CGI will write commands that are submitted
        # by users, so it must be writeable by the user that the web server
        # is running as (usually 'nobody').  Permissions should be set at the 
        # directory level instead of on the file, as the file is deleted every
        # time its contents are processed.
        
        command_file=/usr/local/nagios/var/rw/nagios.cmd

    即要求管道文件\ ``/usr/local/nagios/var/rw/nagios.cmd``\可以被Web服务器读写，显然Web服务器不是以用户nagios的身份来运行的，（一般是apache）。所以将此目录所属的工作组改为apache。

    .. code-block:: bash

        chown :apache /usr/local/nagios/var/rw/nagios.cmd

    即可。重新操作可以正常完成。
