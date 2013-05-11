Nginx指令介绍
***************

详细信息请查看\ `Nginx`_ 原文档(English[#ref1]_, 淘宝中文版\ [#ref2]_)

.. _Nginx: http://nginx.org/

Proxy\_cache相关指令
======================
proxy\_cache
--------------

.. sourcecode:: nginx

    prox_cache zone_name | off;

为cache指定一个共享存储区。相同的存储区可以在不同的地方使用。如果设定为\ *off*\
可以关闭从上级配置中继承的cache定义。

共享存储区域"*zone\_name*"是由指令\ ``proxy_cache_path``\ 定义的。

proxy\_cache\_path
--------------------

.. sourcecode:: nginx

    proxy_cache_path  path [levels=levels] keys_zone=name:size
                        [inactive=time] [max_size=size] [loader_files=number]
                        [loader_sleep=time] [loader_threshold=time];

设定缓存的存储路径和其他一些参数。被缓存的数据都存储在文件中。存储在缓存区的key\
和文件名是通过被代理的URL的MD5值处理后得到的。参数`levels`用于定义缓存的存放级\
别的。例如：下面的指令：

.. sourcecode:: nginx

    proxy_cache_path /data/nginx/cache *levels*=1:2 keys_zone=one:10m;

被缓存的文件在缓存区中是这样存放的:

    /data/nginx/cache/*c*/*29*/b7f54b2df7773722d382f4809d650*29c*

A cached response is first written to a temporary file, then a file is renamed. Starting from version 0.8.9 temporary files and the cache can be put on different file systems but be aware that in this case a file is copied across two file systems instead of the cheap rename operation. It is thus recommended that for any given location both cache and a directory holding temporary files set by the proxy\_temp\_path directive are put on the same file system.
缓存的响应首先写入到一个临时文件，然后进行重命名。

In addition, all active keys and information about data are stored in a shared memory zone, whose name and size are configured by the keys\_zone parameter. Cached data that are not accessed during the time specified by the inactive parameter get removed from the cache regardless of their freshness. By default, inactive is set to 10 minutes.

The special process “cache manager” monitors the maximum cache size set by the max\_size parameter; when this size is exceeded it removes the least recently used data.

A minute after the start the special process “cache loader” is activated that loads information about previously cached data stored on file system into a cache zone. A load is done in iterations. During one iteration no more than loader\_files items are loaded (by default, 100). Besides, the duration of one iteration is limited by the loader\_threshold parameter (by default, 200 milliseconds). A pause is made between iterations, configured by the loader\_sleep parameter (by default, 50 milliseconds).

proxy\_cache\_bypass
----------------------

.. sourcecode:: nginx

    proxy_cache_bypass string ...;

定义在哪些情况下，响应数据不从缓存中取得。当至少有一个字符参数不为空("")且不\
等于"0"，响应就不会从缓存中提取。

``proxy_cache_bypass``\ 的功能与下面的\ ``proxy_no_cache``\ 相同。

proxy\_no\_cache
------------------

.. sourcecode:: nginx

    proxy_no_cache string ...;

与\ ``proxy_cache_bypass``\ 相同。

proxy\_cache\_key
-------------------

.. sourcecode:: nginx

    proxy_cacke_key $scheme$proxy_host$request_uri;

proxy\_cache\_valid
--------------------------

.. sourcecode:: nginx

    proxy_cache_valid 200 302 10m;
    proxy_cache_valid 404 1m;
    proxy_cache_valid any 1m;

proxy\_cache\_lock
-------------------

.. sourcecode:: nginx

    proxy_cache_lock on | off

proxy\_cache\_lock\_timeout
-----------------------------

.. sourcecode:: nginx

    proxy_cache_lock_timeout 5s

proxy\_cache\_min\_uses
------------------------

.. sourcecode:: nginx

    proxy_cache_min_uses 1;

proxy\_cache\_use\_stale
-------------------------

.. sourcecode:: nginx

    proxy_cache_use_stale error | timeout | invalid_header | updating | http_500
                            | http_502 | http_503 | http_504 | http_404 | off ...;

proxy\_cache\_methods
--------------------------

.. sourcecode:: nginx

    proxy_cache_methods GET | HEAD | POST ...;

问题
=====
* **proxy cache**\ 的流程是什么样的？
* ``proxy_cache``, ``proxy_temp_path``, ``proxy_store``\ 三种存储有什么差别？

参考资料
==========
.. [#ref1] http://nginx.org/en/docs/http/ngx_http_proxy_module.html
.. [#ref2] http://tengine.taobao.org/nginx_docs/cn/docs/http/ngx_http_proxy_module.html
