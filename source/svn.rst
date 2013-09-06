SVN Q/A
*********

问题集
=======
# "**Can't open activity db: No such file or directory**"
    这个是因为版本兼容引起的问题。\ `svnadmin create`\ 一个新的repo时，没有在
    repo相应的目录下建立一个"**dav**"目录。只需要在对应repo目录上建立一个"**d\
    av**"目录，并修改一下权限就可以。\ [#ref1]_

    .. sourcecode:: shell

        svnadmin create devops
        mkdir -p devops/dav
        chown -R apache:apache devops

参数资料
=========
.. [#ref1] http://www.johngirvin.com/archives/subversion-cant-open-activity-db.html
