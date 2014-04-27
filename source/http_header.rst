HTTP Header
*************


这里关注的HTTP包括：\ [#rfc]_\ [#book]_

1.  :ref:`Last-Modified <title_last>`\ 和\ ``If-Modified-Since``
2.  :ref:`ETag <title_etag>`\ 和\ ``If-None-Match``, ``If-Match``, ``If-Range``
3.  :ref:`Expires <title_expires>`\ 和\ ``Cache-Control``



.. _title_last:

Last-Modified和If-Modified-Since
=================================
``Last-Modified``\ 是由Web\ **服务器响应请求**\ 时生成，用来验证客户端缓存有效\
性的一个标识。\ [#last]_\ Web服务器通常会在静态文件的HTTP响应头中自动加上此部分。
并被浏览器保存，当浏览器再次请求此内容时，会在HTTP请求中加上\
``If-Modified-Since``\ 头，WEB服务器收到这样一个请求后会与被请求资源的最新修改\
时间进行对比，如果一致就会返回响应码\ ``304`` (Not Modified)，反之会正常返回请\
求内容。对于动态内容，WEB服务器不会主动添加\ ``Last-Modified``\ 头。

.. _title_etag:

ETag和If-None-Match
====================
``ETag``\ 同样是由WEB服务器生成的一个字符串，用于验证客户端缓存有效性的标识。\
[#etag]_\ ``ETag``\ 内容的格式与生成算法RFC并没有规定，由WEB服务器自行实现。对\
于表述内容WEB服务器通常也会主动生成\ ``ETag``\ 头。对于包含\ ``ETag``\ 头的内容，
浏览器在下次请求时会加上请求头\ ``If-None-Match``\ ，内容为响应头中的\ ``ETag``\
内容，当WEB服务器收到这样一个包含\ ``If-None-Match``\ 的请求时，会验证其值与请\
求内容的 ``ETag``\ 值是否一致，如果一致直接返回状态码\ ``304 (Not Modified)``\
，反之正常返回。

通过时间来判断缓存是否有效存在着巨大的不确定性，如服务器时间不正确，与客户端时\
间不一致；文件经常经常更新但是内容没有变化；对于负载均衡架构，后端服务器上文件\
时间很难保证完全一致，而用户请求被随机（或其它策略）分配到不同服务器，会导致用\
户每次都必须获取新的内容。如果使用\ ``ETag``\ 来作为缓存有效性判断标识则可以避\
免这些问题。

.. note::

    对于不同的判断依据，其判断顺序如何？与浏览器的实现是否有关？-> RFC中是否有规
    定？


.. _title_expires:

Expires和Cache-Control
======================
RFC中\ ``Expires``\ 头的格式定义为\ [#expires]_::

    Expires = "Expires" ":" HTTP-date

例如：::

    Expires: Thu, 01 Dec 1994 16:00:00 GMT

``Expires``\ 头中的日期告诉浏览器该内容什么时候过期，暗示浏览器在内容过期之前不
需要再询问服务器，直接使用本地缓存即可。

``Expires``\ 指示的时间为绝对时间。由于时间是由服务器设定，即与服务器时间相关，
所以存在着与\ ``Last-Modified``\ 头类似的问题——当服务端时间与客户端不一致时，就
会引起问题。而另外一个指令（HTTP请求头）\ ``Cache-Control``\ 可以解决此问题。

``Cache-Control``\ 指令包含有大量的参数，详情请查看RFC2626\ [#rfc]_\ [#cache]_::

    Cache-Control: max-age=<second>

``Cache-Control: max-age=3600``\ 可以给缓存内容设定一个相对可欺时间，这个时间是\
相对于客户端的，单位为秒。

事实上，对于Apache服务器，当服务器开启Expires时，会自动添加\ ``Cache-Control``\
响应头。

在默认情况下，Apache服务器没有开启Expires功能，需要进行下面的配置开启Expires功\
能：

.. sourcecode:: apache

    <IfModule mod_expires.c>
        ExpiresActive On
        ExpiresByType   image/gif   "access plus 1 month"
        ExpiresDefault  "now plus 1 day"
    </IfModule>


参考资料
========
.. [#rfc]   :rfc:`2616`
.. [#book]  `构建高性能WEB站点 <http://book.douban.com/subject/10812787/>`_
.. [#last]  `RFC2616 Last-Modified Dates <http://tools.ietf.org/html/rfc2616#page-86>`_
.. [#etag]  `RFC2616 3.11 Entity Tags <http://tools.ietf.org/html/rfc2616#section-3.11>`_
.. [#expires]   `RFC 2616 Expires <http://tools.ietf.org/html/rfc2616#page-127>`_
.. [#cache] `RFC2626 Cache-Control
   <http://tools.ietf.org/html/rfc2616#page-108>`_
