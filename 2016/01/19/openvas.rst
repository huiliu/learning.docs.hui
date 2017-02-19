学习弱点扫描工具-openVAS
************************



.. author:: default
.. categories:: security
.. tags:: devops, security, network
.. comments::
.. more::



安装
=====
在\ **Gentoo**\ 上可以直接使用\ `emerge`\ 来安装：\ ::

    # emerge -qv net-analyzer/openvas

安装完成后，

1.  首先需要运行命令\ `openvas-nvt-sync`\ 来更新弱点数据文件。\ ::

        openvas-nvt-sync

2.  建立一个管理用户\ ::

        openvasad -c 'add_user' -u liuhui -w liuhui -r 'Admin'

3.  创建服务端证书和客户端证书。\ [#]_

    .. code-block:: bash

        # 此步骤非常重要，om为内部使用帐户，如果没有此帐户数据库信息将无法更新
        openvas-mkcert-client -n om -i
        openvas-mkcert
        openvas-mkcert-client

4.  创建一个用户：\ ::

        openvas-adduser

5.  更新Feed\ ::

        openvas-nvt-sync
        openvas-scapdata-sync

6.  重建数据文件：\ ::

        openvasmd --rebuild

7.  启动相关的服务：\ `openvassd`, `openvasad`, `openvasmd`, `gsad`\ 。不同的系统可能不同。Gentoo使用sysvinit时，可以使用\ `/etc/init.d/openvassd start`\ 来启动相关的服务，如果使用\ `systemd`\ ，可以需要自已创建systemd的启动脚本。

8.  进入到\ **openvas**\ Web界面进行扫描。https://localhost

参考资料
==========
.. [#]  http://lists.wald.intevation.org/pipermail/openvas-discuss/2010-May/001952.html
