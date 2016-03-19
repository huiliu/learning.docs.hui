邮件系统-Postfix, Dovecot, MySQL和Postfixadmin
*************************************************

.. author:: default
.. categories:: devops
.. tags:: devops, security, network, mail, software, tips
.. comments::
.. more::




mailbox和maildir
===================
在\ **postfix**\ 的配置文件"*/etc/postfix/main.cf*"中变量\ **home_mailbox**\ 是\
用来设定用户邮件存放地址的。它有两种形式：

Q&A
=====
1.  利用\ **postfixadmin**\ 建立用户，同时会创建相关目录。但是默认情形下，\
    **postfixadmin**\ 创建的目录为"`$virtual_mailbox_base`/`virtual_mailbox_maps`\
    /**username\@domain**"。如果邮件服务器上有多个域名，这样则不便于管理，最好是:\
    "`$virtual_mailbox_base`/`$virtual_mailbox_maps`/**domain/user/**"的形式。

    1. 修改\ **postfixadmin**\ 的配置文件("`config.inc.php`")：
    2. 将\ `$CONF['domain_path']`\ 设定为"**YES**"；
    3. 将\ `$CONF['domain_in_mailbox']`\ 设定为"**YES**".
    4. 使用postfixadmin新建一个用户（即"Add Mailbox"），查询数据库会发现表\
       *mailbox*\ 中用户的"*maildir*"已经是"*domain/user*"格式。
    5. 调整\ **dovecot**\ 的设置：
        * 根据需要修改\ *dovecot-sql.conf.ext*\ 中用户认证的SQL语句。认证用户时\
          使用使用"username@domain"还是”username"形式；查询返回"*maildir*"的路\
          径应该为完整路径
        * 修改用户mailbox的设定：\ **maibox_location** *= maildir:/path/%d/%u/*\
          在文件"*dovecot/conf.d/10-mail.conf*"中。mailbox与maildir的差别请查看。

.. code-block:: php

    <?php

    // ......

    // Mailboxes
    // If you want to store the mailboxes per domain set this to 'YES'.
    // Examples:
    //   YES: /usr/local/virtual/domain.tld/username@domain.tld
    //   NO:  /usr/local/virtual/username@domain.tld
    //$CONF['domain_path'] = 'NO';
    $CONF['domain_path'] = 'YES';
    // If you don't want to have the domain in your mailbox set this to 'NO'.
    // Examples: 
    //   YES: /usr/local/virtual/domain.tld/username@domain.tld
    //   NO:  /usr/local/virtual/domain.tld/username
    // Note: If $CONF['domain_path'] is set to NO, this setting will be forced to YES.
    //$CONF['domain_in_mailbox'] = 'YES';
    $CONF['domain_in_mailbox'] = 'NO';

    // ......
    ?>

参考资料
=========
1. http://www.187299.com/archives/1118 
