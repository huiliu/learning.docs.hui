命令行管道编程
***************


.. author:: default
.. categories:: program
.. tags:: python, program, shell
.. comments::
.. more::


在Shell命令中“\ `grep`, `sed`, `awk`, `cat`, `cut`, `uniq`, `sort`, ...”等都可以使用管道，即，直接使用上一个命令的输出作为当前命令的输入，而不使用中间文件。如：

.. code-block:: bash

    # 使用一长串命令，可以直接得到当前哪个IP访问最多
    awk '{print $1}' /var/log/httpd/access_log | sort | uniq -c | sort | head -10

那么如何写这样一个可以使用管道功能的程序呢？简单看一下\ `head`\ 等的源码就可以得到结果。

C中的实现
===========

.. code-block:: c

    #include <stdio.h>
    #include <stdlib.h> 
    
    int
    main(int argc, char **argv)
    {
        char buf[10];
    
        fread(buf, sizeof(buf), 1, stdin);
        fprintf(stdout, "%s\n", buf);
        return 0;
    }


Shell中的实现
===============


.. code-block:: bash

    #!/bin/bash
    
    #tr '[:lower:]' '[:upper:]' < /dev/stdin
    input=`cat < /dev/stdin`
    echo $input | awk '{print $1}'


Python中的实现
================

.. sourcecode:: python

    #!/bin/env python

    import sys

    f = open("/tmp/access_log", "r")
    f.write(sys.stdin.read())
    f.close()
