SVN Q/A
*********

问题集
=======
1.  "**Can't open activity db: No such file or directory**"

    这个是因为版本兼容引起的问题。\ `svnadmin create`\ 一个新的repo时，没有在
    repo相应的目录下建立一个"**dav**"目录。只需要在对应repo目录上建立一个"**d\
    av**"目录，并修改一下权限就可以。\ [#]_

    .. sourcecode:: bash

        svnadmin create devops
        mkdir -p devops/dav
        chown -R apache:apache devops

2.  利用\ `svnsync`\ 建立SVN仓库镜像时："**svnsync: DAV request failed; it's \
    possible that the repository's pre-revprop-change hook either failed or is\
    non-existent**"

    要求源仓库和镜像仓库中都必须有“\ *pre-revprop-change*\ ”才算合格。

3.  "**Could not open the requested SVN filesystem**, **errcode="13"**"

    这个问题可能是由于SELinux所引起的，按照\ */etc/httpd/conf.d/subversion.conf*
    中的说明操作可以避免。

    ::

        chcon -R -t httpd_sys_content_t repos

4.  在SVN仓库中设定勾子进行SVN实时同步时发生错误："**svnsync: OPTIONS of .... \
    could not connect to server**"。

    在某个仓库的勾子“\ **post-commit**\ ”中添加“\ `svnsync sync http://url/bak/\
    repo`\ ”当进行checkin时，主仓库和镜像仓库会自动同步，但出现了上面的错误。引\
    起这个问题同样是因为\ **SELinux**\ 。查看日志文件“\ */var/log/audit/audit.l\
    og*\ ”，你会看到：

    ::

        type=AVC msg=audit(1379518173.188:94): avc:  denied  { name_connect } for  pid=1498 comm="svnsync" dest=80 scontext=unconfined_u:system_r:httpd_sys_script_t:s0 tcontext=system_u:object_r:http_port_t:s0 tclass=tcp_socket

    通过使用SELinux相关工具（\ `audit2why`\ ）可以找到解决方案：

    ::

        setsebool -P allow_ypbind 1
        setsebool -P httpd_can_network_connect 1

.. Todo::

    需要进行一步学习关于SELinux管理的知识，audit2why属于包setroubleshoot


参数资料
=========
.. [#] http://www.johngirvin.com/archives/subversion-cant-open-activity-db.html
