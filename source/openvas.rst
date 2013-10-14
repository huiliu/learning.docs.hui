学习弱点扫描工具-openVAS
************************

安装
=====
在\ **Gentoo**\ 上可以直接使用\ `emerge`\ 来安装：\ ::

    # emerge -qv net-analyzer/openvas

安装完成后，

1.  首先需要运行命令\ `openvas-nvt-sync`\ 来更新弱点数据文件。\ ::

        # openvas-nvt-sync

2.  建立一个管理用户\ ::

        # openvasad -c 'add_user' -u liuhui -w liuhui -r 'Admin'

3.  创建服务端证书和客户端证书\ ::

        openvas-mkcert
        openvas-mkcert-client

4.  创建一个用户：\ ::

        openvas-adduser

5.  重建数据文件：\ ::

        openvasmd --rebuild


