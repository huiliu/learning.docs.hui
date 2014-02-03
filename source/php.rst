PHP点滴
*********

session
========
PHP\ `Session`_\ 主要用于支持跨域访问。如果服务端配置不正确可能导致一些问题，如用户无法登陆等。\ ``Session``\ 可以保存在客户端cookie中，也可以保存在服务端。默认情况下，PHP会将\ ``Session``\ 保存在磁盘上。其路径由php.ini中的\ ``session.save_path``\ 中。

.. sourcecode:: ini

    session.save_path = '[N[;mode];]path'
    # N 表示目录层级。注意如果使用N且N > 0则不会自动执行垃圾回收；程序不会主动创建目录，需要手动建立各级目录
    # mode 默认为600，修改mode值并不影响进程的umask值。

另外也可以在PHP代码中调用\ ``session_save_path``\ 这个函数来设定Session的保存路径；

对于PHP-FPM，还可以在其配置文件中通过\ ``php_value[session.save_path] = path``\ 来设定此值。

.. _Session:    http://www.php.net/manual/zh/intro.session.php

参考资料
=========
1.  `PHP Session Extensions <http://www.php.net/manual/en/refs.basic.session.php>`_
