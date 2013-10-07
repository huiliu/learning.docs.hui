一个字符引发的问题 - sscanf
***************************
有下面这样一小段测试程序：

.. code-block:: c


    #include <stdio.h>

    int
    main(int argc, char **argv)
    {
        char *buff;
        char    ip[17] = "\0",
                dim,
                username[17] = "\0",
                datatime[22],
                timezone[7];

        buff = "192.168.122.1 - - [17/Sep/2013:21:47:24 +0800] \"GET / HTTP/1.1\" 403 5039 \"-\" \"Mozilla/5.0 (X11; Linux x86_64; rv:23.0) Gecko/20130817 Firefox/23.0\"\n";
        fprintf(stdout, "%s\n", buff);
    
        sscanf(buff, "%s %s %s [%s %s", ip, dim, username, datatime, timezone);
        fprintf(stdout, "%s%s%s%s%s\n", ip, dim, username, datatime, timezone);
        return 0;
    }

1.  在CentOS6.4中使用\ **gcc 4.4**\ 编译可以得到正确的结果：

    192.168.122.1 - - [17/Sep/2013:21:47:24 +0800] "GET / HTTP/1.1" 403 5039 "-" "Mozilla/5.0 (X11; Linux x86_64; rv:23.0) Gecko/20130817 Firefox/23.0"

    192.168.122.1--17/Sep/2013:21:47:24+0800]

2.  在Gentoo上使用\ **gcc 4.7**\ 编译得到的结果是：

    192.168.122.1 - - [17/Sep/2013:21:47:24 +0800] "GET / HTTP/1.1" 403 5039 "-" "Mozilla/5.0 (X11; Linux x86_64; rv:23.0) Gecko/20130817 Firefox/23.0"
    "\000\071\062.168.122.1\000\000"

    192.168.122.1-\ **+0800]**\ -17/Sep/2013:21:47:24+0800]

3.  在Gentoo上使用\ **clang 3.3**\ 编译运行得到：

    192.168.122.1 - - [17/Sep/2013:21:47:24 +0800] "GET / HTTP/1.1" 403 5039 "-" "Mozilla/5.0 (X11; Linux x86_64; rv:23.0) Gecko/20130817 Firefox/23.0"

    --17/Sep/2013:21:47:24+0800]

**原因：**

在邮件列表shlug@googlegroups.com中得到了Yu Changyuan和Ray Song的指点：

    sscanf用法错了。
    
    在sscanf里面，%s对应的指针必须能放字符串加一个'\0'的长度。
    
    man sscanf里面的一段：
           s     Matches a sequence of non-white-space characters; the next
                 pointer must be a pointer to character array that is long enough
                 to hold the input sequence and the terminating null byte ('\0'),
                 which is added automatically.  The input string stops at white
                 space or at the maximum field width, whichever occurs first.
