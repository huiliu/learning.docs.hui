SVN Q/A
*********

问题集
=======
1.  "**Can't open activity db: No such file or directory**"

    这个是因为版本兼容引起的问题。\ `svnadmin create`\ 一个新的repo时，没有在
    repo相应的目录下建立一个"**dav**"目录。只需要在对应repo目录上建立一个"**d\
    av**"目录，并修改一下权限就可以。\ [#ref1]_

    .. sourcecode:: bash

        svnadmin create devops
        mkdir -p devops/dav
        chown -R apache:apache devops

2.  利用\ `svnsync`\ 建立SVN仓库镜像时："**svnsync: DAV request failed; it's \
    possible that the repository's pre-revprop-change hook either failed or is\
    non-existent**"

    要求源仓库和镜像仓库中都必须有“\ *pre-revprop-change*\ ”才算合格。

3.  另外还有一个问题是由于SELinux所引起的，严格按照\ */etc/httpd/conf.d/subversion.conf\ *中的说明操作可以避免问题

参数资料
=========
.. [#ref1] http://www.johngirvin.com/archives/subversion-cant-open-activity-db.html
