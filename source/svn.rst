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

5.  问题描述：

    *   客户端取SVN时指示\ **Cannot open the request svn filesystem**\
        (记不很清),\ **errcode=70014**
    *   检查http日志发现：\ `access_log`\ 中相应的请求都是返回500；\
        `error_log`\ 中提示“\ *(20014)Internal error ...  End of file found*\ ”\
        “\ *Could not fetch resource information.  [500, #0] Could not open the\
        requested SVN filesystem  [500, #70014]*\ ”
    
    解决：google后发现，是因为相应仓库下的文件\ ``db/current``\ 和\
    ``db\txn-current``\ 被坏所引起的。\ [#]_\ ``db/current``\ 存放的是当前修订\
    号；\ ``db/txn-current``\ 应该是存放提交次数之类的数据，此文件为空时，客户\
    端可以正常获取SVN，但提交时会提示\ **../db/txn-current end of file**\ 之类\
    的错误。只需要向其中输入一个换行符(``echo > db/txn-current``)就能解决此问题。

6.  命令\ ``svn info``\ 可以正常执行，但是执行\ ``svn up``\ 时得到错误：\
    ``svn: XML data was not well-formed``

    subversion 1.8时：\ ``svn info``\ 可以正常运行，\ ``svn up``\ 得到错误：\
    ``Updating '.':
    svn: E175004: The PROPFIND response did not include the requested properties``

    原因：在OS X上使用svn时会提示key的解密，奇怪的行为。（用户由管理员降为普通\
    用户，删除过\ *.subversion*\ ，移除过\ *.ssh*\ 均未奏效）

7.  在Linux下利用\ *crontab*\ 定期更新一个SVN仓库时出错：

    .. sourcecode:: text

        svn: Can't convert string from 'UTF-8' to native encoding:
        svn: /home/artsvn/trunk/icloud/D.?\233?\129?\147?\229?\133?\183

    由于中文引起的乱码问题，在\ *crontab*\ 中可以设置环境变量\
    ``LANG=zh_CN.UTF-8, LC_ALL=zh_CN.UTF-8``\ 就可以了。


参数资料
=========
.. [#]  http://www.johngirvin.com/archives/subversion-cant-open-activity-db.html
.. [#]  http://www.cnblogs.com/vegaliming/archive/2012/05/01/2478351.html
        http://www.byywee.com/page/M0/S728/728203.html

