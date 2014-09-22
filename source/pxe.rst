通过PXE安装Linux
*****************
本文描述的操作在Gentoo x86_64上，利用KVM虚拟机完成。

所需软件包有：
*   虚拟机管理软件：app-emulation/libvirt

*   **DHCP服务器**\ ：net-dns/dnsmasq（此工具是libvirt默认的DNS和DHCP工具，在其\
    它环境中应该安装DHCP服务器）

*   **tftp服务器**\ ：net-ftp/atftp
*   **FTP服务器**\ ：net-ftp/vsftpd
*   sys-boot/syslinux
*   CentOS安装DVD

其中DHCP, tftp, ftp服务器是\ **必须**\ 的。

配置DHCP服务器
===============
由于使用的是livirt的默认配置，所以此外关于DHCP的配置与标准DHCP服务器的配置有所
不同。\ [#libvirt_dhcp_bootp]_

libvirtd虚拟机
--------------

运行命令\ ``virsh net-edit default``\ ，上面的命令会用vim打开defautl网络的配置\
文件，请在<dhcp></dhcp>中增加：\
``<bootp file='pxelinux.0' server='192.168.122.1'/>``\ ，以指明tftp服务器地址
为：\ ``192.168.122.1``\ 。关于\ ``bootp``\ 的意义请查看libvirt的网络配置文件
说明。\ [#libvirt_network_xml]_

然后（重新）启动libvirtd服务器：\ ``systemctl restart libvirtd``

单独的dhcpd服务
---------------
如果使用的是独立的DHCP服务器，请参考鸟哥及其它说明。\ [#dhcp_setup1]_\
[#dhcp_setup2]_  \ 。其中关键是在DHCP服务器上指明tftp服务器的地址和读取的文件名。

.. sourcecode:: linux-config
    :emphasize-lines: 6,7

    # cat /etc/dhcp/dhcpd.conf 
    subnet 192.168.122.0 netmask 255.255.255.0 {
        range 192.168.122.2 192.168.122.254;
        default-lease-time 600;
        max-lease-time 7200;
        next-server 192.168.122.1; # PXE Server地址
        filename "pxelinux.0";      # 引导文件名
    }

windows DHCP服务器
------------------
如果使用的是windows DHCP服务器，请按以下步骤设定\ ``next-server``\ （启动服务器\
主机名，启动文件名）值：\ [#win_dhcpd1]_\ [#win_dhcpd2]_

*   打开DHCP管理界面
*   右键单击运行中的DHCP服务器，选择“\ ``设置预定义的选项...``\ ，将会弹出一个\
    窗口
*   第一个下拉栏（选项类别）选择“\ ``DHCP标准选项``\ ”，第二个下拉栏（选项名）\
    选择\ ``066 启动服务器主机名``\ 在值中填入tftp服务器IP，确定。
*   重复上一步，第二个下拉栏（选项名）选择“\ ``067 启动文件名``\ ”，填上\
    ``pxelinux.0``\ （与后面设定的文件名有关），确定。
*   右键单击右侧树形栏中“作用域－作用域选项”，选择“\ ``配置选项``\ “，勾选'\
    ``066, 067``\ '两个选项，然后确定
*   此时可以看到”作用域选项“的右侧栏中有”\ ``066 启动服务器主机名,
    067启动文件名``\ 两个选项
*   配置完成。

配置tftp服务器及安装相关文件
=============================
*   确认已安装\ `net-ftp/atftp, net-ftp/vsftpd, sys-boot/syslinux`\ 。
*   设置并确认tftp, ftp服务器的根目录。tftp服务器的根目录通过文件\
    `/etc/conf.d/atftp`\ 或\
    `/etc/systemd/system/atftp.service.d/00gentoo.conf`\ 来配置。在这里将tftp
    的根目录设定为\ `/tmp/tftproot`\ ，ftp的根目录为：\ `/home/ftp`
*   将CentOS的光盘镜像文件挂载至ftp下：\ ``mount -o loop xxx.iso /home/ftp``
*   准备需tftp提供的文件：
    
    .. sourcecode:: bash

        cd /usr/share/syslinux
        # 其中menu.c32, vesamenu.c32提供菜单介面；pxelinux.0是开机管理程序
        cp pxelinux.0 menu.c32 vesamenu.c32 /tmp/tftpboot

        # 目录pxelinux.cfg用于存放开机菜单
        mkdir -p /tmp/tftproot/pxelinux.cfg
        touch /tmp/tftproot/pxelinux.cfg/default

        # 将CentOS DVD中的引导文件复制
        mkdir -p /tmp/tftproot/kernel/centos64
        # 已将CentOS DVD挂载至/home/ftp
        cd /home/ftp/isolinux/
        cp initrd.img isolinux.cfg vmlinuz /tmp/tftproot/kernel/centos64

*   定制开机菜单

    *   文件\ `/tmp/tftproot/pxelinux.cfg/default`

    .. sourcecode:: text

        # cat /tmp/tftproot/pxelinux.cfg/default

        UI vesamenu.c32         # 使用vesamenu.c32界面模式
        TIMEOUT 300             # 等待时间，单位0.1s
        DISPLAY ./boot.msg      # 提示欢迎介面
        MENU TITLE Welcome to PXE Server System
        
        LABEL local
          MENU LABEL Boot from local drive
          MENU DEFAULT
          localboot 0
        
        LABEL CentOS 6.4
          MENU LABEL Boot from PXE Server for Install CentOS 6.4
          kernel ./kernel/centos64/vmlinuz
          append initrd=./kernel/centos64/initrd.img
    

    *   欢迎提示内容

    .. sourcecode:: text 

        # cat /tmp/tftproot/boot.msg

        Welcome to PXE Server System

*   启动tftp, ftp服务并确认:

    .. sourcecode:: bash

        systemctl start atftp
        systemctl start vsftpd

        # 检查相应端口已打开：tftp服务器侦听UDP 69端口，FTP侦听TCP 21
        netstat -ltunp

安装系统
========
打开\ ``virt-manager``\ 创建一个新虚拟机，并选择从PXE安装系统，正常情况下很快
会进入default提供的菜单介面。如果没有，请确认DHCP服务正常，tftp文件可以正常访
问。另外，请确认虚拟网卡接到了正常的位置（我因为虚拟了好几个，结果接错了接口，
折腾了半天）。

顺利进行安装介面后选择通过网络安装，将地址指向FTP服务器即可。


涉及的知识
===========
*   PXE, 网卡上的\ `ipxe <http://ipxe.org/>`_\ 固件命令
*   tftp服务（如果是CentOS系统，使用的是xinetd服务，进一步可以有hosts.allow等
    ）
*   DHCP服务器的bootp相关，libvirt的网络XML配置
*   ftp服务
*   `syslinux <http://www.syslinux.org>`_\ 工具


参考资料
=========
.. [#libvirt_dhcp_bootp]  `Setting up a TFTP server, PXE boot server with libvirt and virt-manager for ovirt-node <http://dougsland.livejournal.com/123242.html>`_
.. [#libvirt_network_xml]   `libvirt Network XML format <http://libvirt.org/formatnetwork.html>`_
.. [#dhcp_setup1]   `CentOS6.4 x86_64 kvm+PXE备忘录 <http://kumu-linux.github.io/blog/2013/08/22/kvm/>`_
.. [#dhcp_setup2]   `鸟哥 <http://linux.vbird.org/linux_enterprise/0120installation.php>`_
.. [#win_dhcpd1]    `Win2003 DHCP设置、PXE服务设置、网启WINPE <http://wenku.baidu.com/view/206d87ba1a37f111f1855b01.html>`_
.. [#win_dhcpd2]    `How to Add DHCP PXE Options to Microsoft DHCP Server <http://support.citrix.com/article/CTX115094>`_
