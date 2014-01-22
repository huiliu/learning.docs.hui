配置管理工具-Puppet
************************

Resource Ordering
===================
回想一下手动配置一个Apache服务器要经过那些步骤：

1.  安装软件包\ ``httpd``\
2.  根据需要修改配置文件
3.  启动\ ``httpd``\ 服务

那么使用\ ``puppet``\ 进行配置管理时，应该定义三个相应的资源来执行：

.. sourcecode:: puppet

    # install httpd
    package {"httpd":
        ensure => present,
    }
    file {"/etc/httpd/conf/httpd.conf":
        ensure => present,
        owner => root,
        group => root,
        mode => 0644,
        source => "puppet:///modules/$module_name/httpd.conf",
    }
    service {"httpd":
        ensure => running,
    }

当你实际应用上面的\ ``manifests``\ 时会发现执行失败（当然也可能成功）。原因呢？\
前面提到的手动操作步骤是存在先后顺序的，如果不按照相应的顺序，要么失败，要么结\
果不是预期的。这也就要求\ ``puppet``\ 中定义的资源在执行时应该可以设定先后顺序\
，即\ `Resource Ordering`_\ 。


``puppet``\ 提供了四个关键字和两个符号来实现"**Ordering**"的控制：

1.  ``before``
2.  ``require``
3.  ``notify``
4.  ``subscribe``
5.  ``->``
6.  ``~>``

前面四个关键字在\ ``puppet``\ 中称之为元参数（\ `metaparameters`_\ ），它们可\
以接受引用资源（\ `resource reference`_\ ）作为其值。

用\ ``before``\ 和\ ``require``\ 改写上面的\ ``manifests``\ 以按照手动顺序完成：

.. sourcecode:: puppet

    # install httpd
    package {"httpd":
        ensure => present,
        before => File['/etc/httpd/conf/httpd.conf'],
    }
    file {"/etc/httpd/conf/httpd.conf":
        ensure => present,
        owner => root,
        group => root,
        mode => 0644,
        source => "puppet:///modules/$module_name/httpd.conf",
    }
    service {"httpd":
        ensure => running,
        require => ['/etc/httpd/conf/httpd.conf'],
    }

``notify``\ 是增强版式的\ ``before``\，当使用了\ ``notify``\ 的资源发生变化就会\
主动通知\ ``notify``\ 指向的资源；而\ ``subscribe``\ 是增强版的\ ``require``\ ,\
当\ ``subscribe``\ 指向的资源发生的变更，当前资源就会收到通知。

``->``\ 和\ ``~>``\ 意思犹如流程图，从前往后一步一步完成：

.. sourcecode:: puppet

    # install httpd
    package {"httpd":
        ensure => present,
    }
    file {"/etc/httpd/conf/httpd.conf":
        ensure => present,
        owner => root,
        group => root,
        mode => 0644,
        source => "puppet:///modules/$module_name/httpd.conf",
    }
    service {"httpd":
        ensure => running,
    }

    Package['httpd'] -> File["/etc/httpd/conf/httpd.conf"] -> Service['httpd']




Type
=====

group
------
在大多数平台上只能创建组，对于添加组成员由用户属性来控制。\ **group**\ 类型包含\
以下一些常用的属性：

.. graphviz::

    digraph group {
        edge [labeldistance=0.1];
        group -> provider;
        group -> ensure;
        group -> name;
        group -> gid;
        group -> system;
        group -> forcelocal;

        ensure -> present;
        ensure -> absent;

        provider -> aix;
        provider -> directoryservice;
        provider -> groupadd;
        provider -> ldap;
        provider -> pw;
        provider -> windows_adsi;
    }


host
------
用来管理\ **/etc/hosts**\ 中的host条目。对于MacOS X略有不同。

.. graphviz::

    digraph host {
        rankdir=LR;

        host -> name;
        host -> ensure;
        host -> ip;
        host -> host_aliases;
        host -> provider;
        host -> target;

        ensure -> present;
        ensure -> absent;

        provider -> parsed;

        target -> "/etc/hosts";
    }


user
-----
通常用来管理系统用户，缺少一些管理普通用户的特性。\ **user**\ 类型包含以下一些\
常见的属性：

.. graphviz::

    digraph user {
        user -> provider;
        user -> ensure;
        user -> name;
        user -> uid;
        user -> gid;
        user -> groups;
        user -> home;
        user -> password;
        user -> shell;
        user -> system;
    }

例如


file
-------
管理文件（目录）和它们的属性。\ **file**\ 类型具有以下常用属性：

.. graphviz::

    digraph file {
        file -> path;
        file -> ensure;
        file -> owner;
        file -> group;
        file -> mode;
        file -> target;
        file -> content;
        file -> source;

        ensure -> absent;
        ensure -> present;
        ensure -> file;
        ensure -> directory;
        ensure -> link;

        content -> "a file";
        content -> "a string";
    }


问题解答
=========
* "**certificate B: certificate verify failed: [certificate revoked for**"
    从下面的错误中，可以发现"**certificate revoked for ......**"，由此可以判断\
    应该是证书过期的原因。

    .. sourcecode:: text
    
        Notice: Starting Puppet client version 3.2.4
        Info: Caching certificate_revocation_list for ca
        Warning: Unable to fetch my node definition, but the agent run will continue:
        Warning: SSL_connect returned=1 errno=0 state=SSLv3 read server certificate B: certificate verify failed: [certificate revoked for /CN=puppet.virt.liuhui]
        Info: Retrieving plugin
        Error: /File[/var/lib/puppet/lib]: Failed to generate additional resources using 'eval_generate: SSL_connect returned=1 errno=0 state=SSLv3 read server certificate B: certificate verify failed: [certificate revoked for /CN=puppet.virt.liuhui]
        Error: /File[/var/lib/puppet/lib]: Could not evaluate: SSL_connect returned=1 errno=0 state=SSLv3 read server certificate B: certificate verify failed: [certificate revoked for /CN=puppet.virt.liuhui] Could not retrieve file metadata for puppet://puppet.virt.liuhui/plugins: SSL_connect returned=1 errno=0 state=SSLv3 read server certificate B: certificate verify failed: [certificate revoked for /CN=puppet.virt.liuhui]
        Error: Could not retrieve catalog from remote server: SSL_connect returned=1 errno=0 state=SSLv3 read server certificate B: certificate verify failed: [certificate revoked for /CN=puppet.virt.liuhui]

    但是，在实际操作时，我几乎是同时进行的，前后不差三分钟。还是按照官方文档\
    [#ref1]_\ 中的提示检查了证书有效期：

    .. sourcecode:: bash

       openssl x509 -text -noout -in /var/lib/puppet/ssl/certs/hostname.tld.pem | grep -A2 Validity

    最后发现确实有点问题，日期不是当前日期，而且与"`puppet master`"的日期亦不同\
    步。由此可以推断可能是"`agent`"和"`master`"的时间不同步，而"`agent`"的系统\
    刚好不在"`master`"签发的有效时间内，导致证书无效。由此得到教训：“\ **使用\
    Puppet时一定要保证"master"和"slave"的时间同步**\ ”。

    修正时间同步的问题后，此问题仍然存在，又google了一些讨论\ [#ref2]_\ [#ref3]_
    ：有人提到是"**.ssh**"目录的问题，经查没有此目录，故排除。

    经常反复尝试，发现问题所在，因为使用了三台虚拟机，一个作为Master，两个Slave\
    。发现有一个Slave一直都可以用，不会出上面的错误。但是当先将Master的自管理建\
    立好，然后就再去设定Slave时，这个一直没有问题的Slave也出现相同问题。联想到\
    在Master上运行"`puppet agent --server=host --test`"，总是提示：

    .. sourcecode:: text

        On the master:
            puppet cert clean puppet.virt.liuhui
        On the agent:
            rm -f /etc/puppet/ssl/certs/puppet.virt.liuhui.pem
            puppet agent -t

    相当于重新生成一个证书，而在\ `puppet master`\ 启动时也生成了一套证书的。猜\
    想是不是因为再次的证书混乱导致\ `puppet master`\ 上证书管理混乱？最后发现\
    `puppet master`\ 生成的证书位于"/var/lib/puppet/ssl"下，而\ `puppet agent`\
    生成的证书在"/etc/puppet/ssl"下，但是两次证书的名字一样，使用的CA一样，所以\
    导致\ `puppet master`\ 分不清，搞乱了。

    为什么\ `puppet master`\ 和\ `puppet agent`\ 的证书存放目录不一样呢？检查配\
    置文件"/etc/puppet/puppet.conf"发现，其中只有关于\ `master`\ 的配置，没有\
    `agent`\ 的信息，应该是\ `agent`\ 的默认路径就是在"/etc/puppet/ssl"，而\
    `master`\的证书信息则在"/var/lib/puppet/ssl"。怎么解决呢？在配置文件"`/etc/\
    puppet/puppet.conf`"中添加上关于\ `agent`\ 的配置信息就好了。

    .. sourcecode:: ini

        [agent]
            ssldir = /var/lib/puppet/ssl

问题
=====
如何将\ ``puppet``\ 中的变量值传递给命令行
----------------------------------------------
写了一个module来执行一个编译任务，目录结果如下：

.. code-block:: text

    stackless/
            manifests/
                    init.pp
                    install.pp
                    params.pp

其中文件\ ``params.pp``\ 定义了一些变量，如：

.. code-block:: puppet

    class stackless::params {
        $srcPath = "/home/builder/stackless"
        $installPath = "/home/builder/local"
    }

在\ ``install.pp``\ 中执行相关的编译工作，如：

.. code-block:: puppet

    class stackless::install {
        exec {"configure":
            cwd => $stackless::params::srcPath,
            path => ["/bin", "/usr/bin"],
            command => "chmod 755 configure && ./configure --prefix=$stackless::params::installPath",
        }
    }

然后使用此模块，编译正常完成，但是程序被安装到系统目录\ ``/usr``\ 下面。查看“\
**config.log**\ ”，发现\ ``./configure --prefix=``\ 后面的参数为空。猜想应该时\
``puppet``\ 直接将\ *command*\ 交给了Shell执行，而没有先进行变量替换而导致的问\
题。还没有深入了解是否有其它机制将变量先替换然后再转交给Shell。



参考资料
=========
.. [#ref1] http://projects.puppetlabs.com/projects/1/wiki/certificates_and_security
.. [#ref2] http://smartest.blog.51cto.com/3585938/1016576
.. [#ref3] http://bitcube.co.uk/content/puppet-errors-explained

.. _Resource Ordering: http://docs.puppetlabs.com/learning/ordering.html
.. _metaparameters: http://docs.puppetlabs.com/references/stable/metaparameter.html
.. _resource reference: http://docs.puppetlabs.com/puppet/latest/reference/lang_datatypes.html#resource-references
