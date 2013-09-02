配置管理工具-Puppet
************************



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


参考资料
=========
.. [#ref1] http://projects.puppetlabs.com/projects/1/wiki/certificates_and_security
.. [#ref2] http://smartest.blog.51cto.com/3585938/1016576
.. [#ref3] http://bitcube.co.uk/content/puppet-errors-explained
