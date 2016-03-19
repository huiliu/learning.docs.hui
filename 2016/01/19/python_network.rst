Python网络库的使用
*******************



.. author:: default
.. categories:: python, program
.. tags:: python, program, network, urllib
.. comments::
.. more::



HTTP的POST, GET方法
====================
HTTP常用于向服务端发送数据的方法是：\ ``POST``\ 和\ ``GET``\ 模拟表单提交数据，\
Python库\ ``urllib``\ 中已经有现成的函数可以直接调用。\ [1]_

*   利用\ ``GET``\ 方法发送数据：

    .. code-block:: python

        import urllib
        params = urllib.urlencode({'name':'hello', 'passwd':'world'})
        html = urllib.urlopen('http://www.example.com/login?%s' % params).read

*   使用\ ``POST``\ 方法发送数据：

    .. code-block:: python

        import urllib
        params = urllib.urlencode({'name':'hello', 'passwd':'world'})
        html = urllib.urlopen('http://www.example.com/login?%s', params).read

上面几乎完成一样，只是调用\ ``urllib.urlopen``\ 时使用的参数不一样而已。

下面是一个QQ帐号的钓鱼网站，使用循环POST不断向其发送伪造的数据：

    .. code-block:: python

        http://23.80.192.20/37/qq.com/4/

        import random
        import urllib
        import time
        
        strSample = 'abcdefghijklmnopqrstuvwxyz1234567890_@#$-'
        
        for i in range(99999999):
            ip = "%d.%d.%d.%d" % (random.randint(1,254),random.randint(1,254),random.randint(1,254),random.randint(1,254))
            qq = random.randint(1234567, 12345678900) passLen = random.randint(8,16)
            
            passwd = random.sample(strSample, passLen)
            data = {
                        "ip": ip,
                        "qq": qq,
                        "mima": ''.join(passwd)
                    }
            url = 'http://23.80.192.20/37/%23add7.asp'
            params = urllib.urlencode(data)
            
            html = urllib.urlopen(url, params).read()
            time.sleep(1)

参考说明
========
.. [1]  http://docs.python.org
