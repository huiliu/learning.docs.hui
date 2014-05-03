Best Practices for Speeding Up Your Web Site
********************************************

本文翻译自Yahoo\ [#ref]_

最小化HTTP请求数
================
终端用户80%的响应时间花费在前端。其中花费最多的是下载页面中的各种组件：图片，样\
式表，JavaScript，Flash等。减少页面包含在页面中的组件可以减少客户端在渲染被请求\
页面时的HTTP的请求数。

减少页面组件数的一种方法是简化页面的设计。但是有没有一种方法可以建立包含大量组\
件的页面同时保持更快的响应时间呢？实际上在设计富页面时，有一些方法可以减少HTTP\
的请求次数：

1.  ``合并文件``\ 是一种减少HTTP请求数的方法。\ ``合并文件``\ 即：通过将所有的\
    脚本文件合并为一个脚本文件，同样将所有样式表文件合并为一个文件。当每个页面\
    载入的脚本和样式表不尽相同时，\ ``合并文件``\ 的方法面临着更大挑战，但是应\
    该了解到在发布过程时这是一种改善响应时间的手段。
2.  `CSS Sprites <http://alistapart.com/article/sprites>`_\ 是减少图片请求数的\
    有效方法。将背景图片组合为单个文件并使用CSS的\ ``background-image``\ 和\
    ``background-position``\ 等属性来控制背景图片显示，以期符合期望。
3.  `Image maps <http://www.w3.org/TR/html401/struct/objects.html#h-13.6>`_\ 将\
    多幅图片组合为一张，图片总的大小相同，但是减少了HTTP请求数加快了页面的加载\
    ，只有当图片在页面上处于连续位置时才可以使用\ ``Image maps``\ 方法。例如导\
    航栏。定义\ ``Image maps``\ 的坐标是相当乏味且容易出错，导航使用\ ``Image
    maps``\ 也无法访问，因此并不推荐使用。
4.  ``Inline images``\ 使用\ `data: URL scheme
    <http://tools.ietf.org/html/rfc2397>`_\ 将图片嵌入到页面，这会增大HTML文件\
    。将\ ``inline images``\ 合入到样式表（可缓存的）是一种减少HTTP请求数的方法\
    ，同时可以避免增加页面的大小。需要注意的是\ ``Inline images``\ 方法并不被所\
    有的主流浏览器支持。

减少页面的HTTP请求数是提高性能的第一步，对于改善首次访问的用户的体验是最为重要\
的准则。如Tenni Theurer博文\ `Browser Cache Usage - Exposed!
<http://yuiblog.com/blog/2007/01/04/performance-research-part-2/>`_\ 中描述的：\
网站每天的访问中有40%-60%用户是没有缓存任何数据的，首次访问时更快是更好的用户体\
验的关键。

使用CDN（内容分发网络）
=======================
用户距服务器的距离对\ ``响应时间``\ 有一定影响，因此将内容部署到多个分散的服务器
上将使得用户加载速度更快，但是该从什么地方开始呢？

作为第一步，将内容分散到不同地方的服务器，不必重新设计Web应用让其工作在分布式架\
构下。对于不同的应用，调整架构需要不同的工作量，甚至令人望而生畏。如同步会话状\
态，在不同位置的服务器间复制数据库等。这些都可能让你尝试减少用户与服务器间响应\
时间的努力功亏一篑，永远还要走到调整应用架构这一步。

请铭记：用户响应时间的80%-90%花费在下载页面中的各种组件：图片、样式表、脚本、动\
画等。这是\ *性能黄金准则(Performance Golden Rule)*\ 。与其从重新设计你的应用架\
构这类艰巨的任务开始，不如将你的静态文件分发到CDN，这不仅可以大大减少响应时间，\
而且CDN（的使用）非常简单。

CDN是一系列分散在各地WEB服务器，可以更加高效快捷的向用户提供服务。分发内容到某\
一用户时CDN所选择的服务器通常基于网络近似度，如最小的网络跳数，服务器的最快响应\
时间。

一些大的互联网公司拥有自己的CDN，但是也有一些CDN服务商提供付费服务，如Akamai
Technologies, EdgeCast, level3。对于初创公司和个人站点，CDN的价格可能难以承受，\
但是如果你的受众量不断增大，变得更加全球化，为了达到更快的响应，使用CDN是必须的\
。在Yahoo，将静态内容从各自的应用中剥离并使用CDN（前面提到的第三方CDN服务商和\
Yahoo自己的CDN），使用用户的响应时间提高了20%以上。切换到CDN只需要相对简单的修\
改，但是可以极大的提高网站访问速度。

.. _section_expires:

添加\ ``Expires``\ 或\ ``Cache-Control``\ HTTP头
=================================================
此规则包含两个方面：

1.  对于静态内容，通过将HTTP头\ ``Expires``\ 设定为遥远的将来使其“永不过期”；
2.  对于动态内容，通过合理的设定HTTP头\ ``Cache-Control``\ 为浏览器设定一个请求\
    条件。

现在网页被设计得越来越丰富，这意味着页面中包含更多的脚本、样式表，图片，动画等。
用户第一次访问量不得不进行大量的HTTP请求，通过使用\ ``Expires``\ 头，可以让这些\
内容被客户端缓存，如此一来，用户接下来的访问就可以避免一些不必要的HTTP请求。\
``Expires``\ 头大量应用于图片资源，但是实际上它可以被应用于所有的页面内容，包括\
脚本，样式表，动画等。

浏览器（代理）使用缓存来减少HTTP请求数和大小，使用网页加载更快。WEB服务器通过在\
HTTP响应头中加入\ ``Expires``\ 头来告诉客户端（程序）当前内容可以缓存多久。下面\
是一个Expires时间设定较远的HTTP\ ``Expires``\ 头，它告诉浏览器直到2010年4月15号\
当前返回的内容都是有效的。\ ::

    Expires: Thu, 15 Apr 2010 20:00:00 GMT

如果你使用的是Apache，使用指令\ ``ExpiresDefault``\ 来设定一个相对当前的过期时\
间。下面的例子使用\ ``ExpiresDefault``\ 指令将过期时间设定为请求时间10年后。（\
译者注：\ ``Expires``\ 的设定值与WEB服务器时间有关，如果客户端与服务器时间不一\
致可能引起问题。详细请阅读\ :rfc:`2616`\ ）\ ::

    ExpiresDefault "access plus 10 years"

请注意：如果使用\ ``Expires``\ 将内容设置为永不过期，无论什么时间你对某内容进行\
了更新，都必须更换此资源的文件名。在Yahoo，我们通常在构建步骤中加入这样一步：将\
版本号加入到文件名中。例如：\ *yahoo_2.0.6.js*

只有用户已经访问过你的站点，使用包含一个“永不过期”的\ ``Expires``\ HTTP头才能影\
响页面访问量（PV, page views）。对于初次访问以及注意器缓存为空时，它不会影响（\
减少）HTTP请求数。\ **因此这种性能改进的影响有赖于用户有多大几率是在有缓存情形\
下访问站点。**\ 我们测量了Yahoo站点的这方面数据，发现大约有75%-85%有页面访问量\
是在有缓存情况下进行的。\ **通过使用“永不过期”的Expires头，可以增加被缓存的内容\
，当其它页面被访问时可以重复利用它们而不是向服务器再次请求**

.. _section_gzip:

压缩页面内容(Gzip Components)
=============================== 
通过前端工程师的调优，可以显著的减少HTTP请求和响应在网络上的传输时间。另一方面\
，用户的访问带宽速度，ISP，proximity to peering exchange points等都超出了开发团\
队的控制范围。但是这些都会影响到响应时间，压缩可以减小HTTP响应内容的大小进而减\
少响应时间。

从HTTP/1.1开始，WEB客户端在发起HTTP请求时，在请求头中添加字段\
``Accept-Encoding``\ 来说明客户端支持压缩：\ ::

    Accept-Encoding: gzip,deflate

当WEB服务器从请求头文件中发现了上面内容后，就会对响应内容使用一种由\
``Accept-Encoding``\ 指定的压缩方法进行压缩再返回给客户端。同样WEB服务器会在响应
头也会添加字段\ ``Accept-Encoding``\ 来告诉客户端返回内容使用了什么方法进行压缩
，如：\ ::

    Accept-Encoding: gzip

Gzip是由GNU项目开发的当前最为有效和流行压缩方法，在\ :rfc:`1952`\ 中进行了标准\
化，另一种可以看到的压缩方法是\ ``deflate``\ ，它并不那么的高效和流行。

经过Gzip压缩一般可以将响应内容大小减小70%，当前互联网上通过浏览器产生的流量的\
90%都声称支持gzip压缩。

对于浏览器和代理存在已知的问题：浏览器收到的内容的压缩格式和期望收到的格式不一\
致。幸运的是随着旧浏览器被淘汰，这种边际问题正逐渐减少。Apache的模块通过在响应\
头中自动添加适当的\ ``Vary``\ 值也可以消除这种问题。

WEB服务器基于文件类型来选择性进行压缩，决定压缩什么通常太有限。绝大多数WEB站点\
都会压缩HTML文件。另外对脚本、样式表进行压缩也是值得的，但是大多数站点都没有对\
它们进行压缩。事实上，任何文本内容包括XML，JSON都是值得压缩的。由于图片和PDF文\
件自身已经压缩过，所以它们都不需要压缩，如果对这些文件进行压缩，不仅浪费CPU，而\
且还可能增加文件大小。

使用gzip压缩多种文件类型，是一种减小页面大小和提高用户体验的一种简单方法。

将样式表放在页面顶部
====================
通过在Yahoo的(WEB)性能研究，我们发现将样式表放置在HTML文件的\ ``HEAD``\ 位置时\
，页面加载的更快。这是因为将样式表放在\ ``HEAD``\ 中允许页面逐步进行渲染。

关注性能的前端工程师想要页面是逐步加载，也就是说希望浏览器只要它获取到内容就可\
以进行渲染显示。这对于包含大量内容页面和网络较慢的用户特别重要。一些研究和\
`资料`_\ 显示，（在加载时）给用户以视觉的反馈提示（如进度条）是非常重要的。在我\
们的研究例子中，页面是逐步加载的，浏览器中页面内的头部，导航栏，顶部Logo等被逐\
步加载显示。所以为正在等待加载页面用户提供的视觉反馈，都可以改善所有用户的体验。

将样式表放在页面底部引起的问题是：大多数浏览器（包括IE）会禁止页面逐步渲染显示。
由于在这种情况下，浏览器无法确定上部元素的样式表是否会发生变化，所以为了避免页\
面元素的重复渲染，这些浏览器禁止此种情况下逐步渲染页面。

`HTML规范`_\ 明确指出样式表应该包含在页面的\ ``HEAD``\ ：“与标签\ ``a``\ 不同，\
``link``\ 只能出现在HTML文档的\ ``HEAD``\ 中，然而是可以存在多个\ ``link``\ 标签
。”对于空白页面或不包含样式表的内容，……最佳解决方案是遵循HTML规范，将你的样式表
放在页面的\ ``HEAD``\ 中进行加载。

.. _资料:   http://www.useit.com/papers/responsetime.html
.. _HTML规范:   http://www.w3.org/TR/html4/struct/links.html#h-12.3

将脚本放在页面的底部
====================
（加载）脚本会阻止浏览器的并行下载。\ `HTTP/1.1规范`_\ 中建议浏览器下载内容时每\
个域名并行下载最多为2个。如果你使用多个域名提供图片服务，可以实现超过两个的并行\
下载。当正在下载一个脚本时，即使资料在不同的服务器上，浏览器也不会进行任何其它\
下载。

在某些情形下，脚本不便于放置在页面的底部。例如，如果脚本中使用\
*document.write*\ 向页面中插入内容时就不能将它移动到页面底部。可能还有作用域的\
问题，在许多情况下，也有方法来解决这些问题。

另外一个常见的建议是使用延迟脚本（deferred scripts）。\ ``DEFER``\ 属性表示该脚\
本没有包含\ *document.write*\ 内容，暗示浏览器可以继续进行渲染。不幸的是，\
Firefox不支持\ ``DEFER``\ 属性。对于IE浏览器，脚本可能被延迟，但是并不像期望的\
那样（工作）。如果一个脚本可以被延迟，它就可以被移到页面的底部，这样就可以加快\
网页的加载速度了。

.. _HTTP/1.1规范: http://www.w3.org/Protocols/rfc2616/rfc2616-sec8.html#sec8.1.4

不要使用CSS表达式
=================
CSS表达式是动态设定CSS属性的一种有力方法，同时也一种危险的方法。它们从IE 5开始\
被支持，但是从IE8开始被废弃。下面的例子中，通过使用CSS表达式，将每个小时改变背\
景颜色一次。

.. sourcecode:: css

    background-color: expression( (new Date()).getHours()%2 ? "#B8D4FF" : "#F08A00" );

正如上面展示的，\ ``expression``\ 方法可以接受JavaScript表达式。通过计算\
JavaScript表达式的结果来设定CSS属性值。需要注意的是除IE外的其它浏览器并不支持\
``expression``\ 方法，在IE浏览器中设置CSS属性是非常有用的，对于跨浏览器则需要提\
供相应一致性的体验。

CSS表达式带来的一个问题是远超人们期望的频繁的计算表达式值。它们不仅会在页面渲染\
和改变大小时计算表达式的值，在页面滚动时，甚至是用户在页面上移动鼠标时都会重新\
计算表达式的值。在CSS表达式中增加一个计数器，我们可以方便的追踪CSS表达时在什么\
时候，什么样的频率计算表达式的值，将鼠标在页面内移动一圈计数器的值轻松超过10000.

减少CSS表达式计算次数的一种方法就是使用\ ``一次性（one-time）表达式``\ ，一次性\
表达式在第一次被计算式给相应的属性设定一个显式的值，而不是CSS表达式。如果CSS属\
性必须在页面的生命周期内动态变化，可以\ ``使用事件处理的方式来代替CSS表达式``\
。如果你必须使用CSS表达式，请记住它们可以被重复计算成千上万次，并且会影响页面性\
能。


将JavaScript和CSS内容存储在外部文件
====================================
这里介绍的大多数（改善）性能规则都是在介绍如何管理外部（译者注：HTML页面之外）组
件，然而在考虑这些之前，你可能会考虑一个更基础的问题：“JavaScript, CSS应该存放在
外部文件中还是直接写在HTML页面中呢？”

由于JavaScript和CSS文件可以被浏览器缓存，所以现实中被它们存放在外部文件中（再链
接到相应的HTML文件）通常会使用页面加载的更快。将JavaScript, CSS内容内联到HTML页
面中，每次请求时都会下载，虽然可以相应的减少HTTP请求数，但是增大了HTML页面的大小
。另一方面，存放在外面文件中的JavaScript和CSS如果已经被浏览器缓存，则可以减小
HTML页面的大小，同时也不会增加HTTP请求数。

关键点是：被缓存的JavaScript和CSS文件相对于HTML文件请求次数的频率。这个因素虽然
难以量化，但是可以通过多种方法来测量。如果访问站点的用户每次会话时会浏览多个页面
，并且大多页面重复使用了相同的脚本和样式表，就可以从缓存外部文件中警告巨大的好处
。

大多数站点都在这些指标中，对于这些站点，最好的解决方法通常是将JavaScript和CSS内
容作为外部文件部署。将JavaScript和CSS内联到HTML文件中的唯一的例外是站点主页。例
如Yahoo主页，我的Yahoo。用户每次会话，主页只有较少的页面访问量（可能只有一次），
你可能会发现将JavaScript和CSS内联到页面中时，返回给用户的响应时间更快。

主页通常是用户访问的入口，第一个页面。有一些技术既达到由内联带来的HTTP请求数减少
的效果，也可以拥有缓存外部文件优点。如：将JavaScript和CSS内联在主页中，但是当页
面（主页）加载完成后动态地加载外部文件，后续访问其它页面时就可以直接引用这些已经
被浏览器缓存的外部文件。

.. _section_dns:

减小DNS查找
============
标签：内容

域名系统（Domain Name System, DNS）将主机名映射到IP地址，就像电话薄将人名与他们
的电话号码关联起来一样。当你在浏览器地址栏中输入\ *www.yahoo.com*\ （并按回车）
后，浏览器就会联系DNS解析服务器查询\ *www.yahoo.com*\ 对应的IP地址，进行DNS查询
是有代价的，向DNS服务器查询某个域名（主机名）对应的IP通常会花费20-120毫秒。在完
成DNS查询之前，浏览器无法从目标主机下载任何内容。

缓存DNS查询结果可以获得更好的性能。这样一个缓存通常发生在ISP或者本地局域网中的一
个特殊服务上，但是用户个人计算机也会缓存DNS查询结果，它们被保存在操作系统的DNS缓
存中（Windows系统上的“\ *DNS Client service*\ ”）。另外大多数浏览器也有不同于系
统级的缓存。浏览器保存着自己的DNS缓存，并且对系统中的DNS缓存没有干扰。

对于IE，默认会缓存DNS结果30分钟，这个值可以通过修改注册表中的字段\
``DnsCacheTimeout``\ 调整。Firefox默认缓存DNS结果1分钟，可以通过修改配置选项\
``network.dnsCacheExpiration``\ 来调整。（FasterFox将这个值修改为1小时）

但是客户端DNS缓存为空时（浏览器和操作系统的均为空），DNS查询次数等于页面中唯一主
机的数量，这包括当前页面URL对应的主机，图片，脚本文件，样式表文件，动画对象等。
减少唯一主机名的数量可以减少DNS查询的次数。

减少唯一主机名的数量潜在的也会减少加载页面时并行下载的数量。避免（减少）DNS查找(
（次数）可以降低响应时间，但是减少并行下载会增加响应时间。我的经验是\ **页面组件
分配到至少2个，最多不超过4个域名中，可以在减少DNS查找次数和高的并行下载数间达到
一个很好的平衡。**


.. note::

    将文件分散到2-4个域名下较为合适。当然可以通过调整DNS缓存时间来改善DNS请求次
    数，但是对于开发者来说，客户端因素无法掌控。

.. _section_minify:

减小JavaScript和CSS代码的大小
==============================
缩小是通过删除代码中非必须字符来减小代码大小，从而提高加载速度的一种实践。当对代
码进行缩小时，所有的注释，非必须的空白字符（空格，换行符，制表符）都被删除，在这
种情况下，由于下载的数据减少可以改善JavaScript的响应时间。有两个流行的工具用来缩
小JavaScript代码：\ `JSMin`_\ 和\ `YUI Compressor`_\ 。YUI压缩工具也可以用来缩小
CSS。

``模糊处理(Obfuscation)``\ 是另一种优化源代码的方法。它比缩小都为复杂，也更容易
在处理时产生BUG。一项对美国十大网站的调查显示，\ **缩小可以将文件大小减小21%，模
糊处理可以减小25%**\ ，虽然模数处理可以将文件变得更小，但是缩小JavaScript的风险
更小。

另外不仅外部的脚本和样式表可以被缩小，内联在标签\ ``<script>``\ 中的代码也可以被
缩小。即使你使用gzip对脚本和样式表进行了压缩，缩小仍然可以将将文件大小减少\
``5%``\ 以上。随着JavaScript和CSS不断增加，通过缩小取得的效果将愈加明显。

.. _JSMin:  http://crockford.com/javascript/jsmin
.. _YUI Compressor: https://developer.yahoo.com/yui/compressor/

.. note::

    WEB服务器启用压缩后，文件大小减少在5%（取决于原文件的大小）。

.. _section_redirect:

避免重定向
===========
标签：内容

通过返回状态码\ *301, 302*\ 可以实现重定向。下面是一个WEB服务器响应\ *301*\ 的
HTTP头：

.. sourcecode:: text

    HTTP/1.1 301 Moved Permanently
    Location: http://example.com/newuri
    Content-Type: text/html

浏览器自动将用户带到（HTTP头中）\ ``Location``\ 字段指定的URL，重定向所需的信息
全部都在HTTP响应头中，通常响应内容为空。除非响应头中包含有\ ``Expires``\ 或者\
``Cache-Control``\ 字段控制缓存，否则不论是301还是302响应都不会被缓存。标签\
``meta refresh``\ 和JavaScript都可以将用户引导至一个不同（于地址栏）的URL，但是
如果必须进行重定向，最好是使用标准的3xx HTTP状态码。因为它可以让（浏览器的）后退
按钮正确的工作。

关于重定向最重要的是它会使得用户加载变慢，用户体验变差。对于重定向，直到HTML文件
下载完成，页面中什么都不能被下载和渲染，在用户和目标HTML页面间插入重定向会延缓页
面内的所有内容。

一个开发者通常没有意识到且频繁发生的重定向行为是，当一个URL结尾应该有而没有斜线“
\”时，WEB服务器会返回一个301状态码重定向至一个包含斜线的URL。例如：访问
http://astrology.yahoo.com/astrology，WEB服务器会返回一个包含重定向至
http://astrology.yahoo.com/astrology/的301响应，Apache通过指令\ ``Alias``\ ，或
者模块\ ``mod_rewrite``\ 或指令\ ``DirectorySlash``\ 可以避免这种行为，如果你使
用的是Apache WEB服务器可以使用这些方法避免重定向。

重定向的另一个最常用途是连接一个旧的站点到新站点；另外还用于连接站点的不同部分；
基于某些条件（如浏览器类型，帐户类型等）将用户引导至不同页面。使用重定向连接两个
站点非常的便利，而且对代码只需要很少的改动。虽然在这些情况下使用重定向降低了开发
者的复杂度，但是也使用用户体验变差。如果重定向前后两部分都在同一台服务器上，可以
使用（Apache的）指令\ ``Alias``\ 和URL重写模块\ ``mod_rewrite``\ 来代替重定向。
如果重定向是域名发生了变化，可以在DNS记录中添加\ ``CNAME``\ 记录并组合\
``Alias``\ 和\ ``mod_rewrite``\ 来取代重定向。

.. note::
    
    竭力避免重定向。注意URL结尾斜线可能引起的重定向问题。

删除重复的脚本
===============
标签：javascript

在一个页面内将同一个JavaScript脚本加载再次会对性能产生影响。这并不像你想像的那样
不同寻常。一个对美国前十大网站的研究表明，其中有20%的站点包含有重复脚本。有两方
面的因素增加了在单个页面内重复加载脚本的几率：团队的大小和脚本的数量。当存在重复
脚本时，浏览器需要额外非必需HTTP请求来下载脚本，并重复执行它们，从而影响页面的渲
染性能。

对于IE浏览器，重复的脚本会引起非必需的HTTP请求，Firefox则不会。对于IE浏览器，如
果一个外部的脚本文件被包含两次并且不可缓存，在载入页面的时候浏览器会请求两次。即
使脚本可以被缓存，当用户重新加载页面时也会进行额外HTTP请求。

避免将同一个脚本加载两次的一种方法是：在你的模板系统中声明一个脚本管理模型。通常
的做法是在HTML页面中通过\ ``script``\ 标签来包含一个外部脚本：

.. sourcecode:: html

    <script type="text/javascript" src="menu_1.0.17.js"></script>

在PHP中可以通过一个自定义函数（如下面的\ ``insertScript``\ ）来实现类似功能：

.. sourcecode:: php

    <?php insertScript("menu.js") ?>

为了阻止同一脚本被插入多次，这个函数可以处理一些关于脚本的其它问题，如依赖检查，
将版本号加入到文件名中并通过\ ``Expires``\ 使用脚本“永不过期”。

.. note::

    通过良好的代码管理来避免重复插入脚本。

.. _section_etags:

配置\ ``ETag``\ s
===================
标签：javascript

``实体标签(Entity tags, ETags)``\ 是WEB服务器与浏览器间用来判断浏览器缓存项是否
与原服务器上的数据一致的一种机制。（An "entity" is another word a "component":
images, scripts, stylesheets, etc.）通过添加实体标签来验证内容是否有效比起\
``Last-Modified``\ 日期更加灵活。实体标签对某一特定版本的组件内容是一个唯一字符
串标识。（实体标签内容的）唯一格式要求是\ *字符串必须包含在引号中*\ 。WEB服务器
在响应头中为特定组件添加\ ``ETag``\ 字段。

.. sourcecode:: text

    HTTP/1.1 200 OK
    Last-Modified: Tue, 12 Dec 2006 03:03:59 GMT
    ETag: "10c24bc-4ab-457e1c1f"
    Content-Length: 12195

随后浏览时，如果浏览器必须验证某个内容组件，它会在HTTP请求头中添加\
``If-None-Match``\ 并将\ ``ETag``\ 的值返回给WEB服务器。如果实体标签（与WEB服务
器上文件的）匹配，WEB服务器就会返回一个304状态码（而不返回文件数据），针对上面的
例子，HTTP响应可以减小12195个字节。

.. sourcecode:: text

    GET /i/yahoo.gif HTTP/1.1
    Host: us.yimg.com
    If-Modified-Since: Tue, 12 Dec 2006 03:03:59 GMT
    If-None-Match: "10c24bc-4ab-457e1c1f"
    HTTP/1.1 304 Not Modified

实例标签的问题是WEB服务器在生成时，其值通常与存放站点文件的服务器相关。当浏览器
从一个服务器上取得原始数据，然后到另外一个服务器上验证时，实体标签将不会匹配。而
这种情况对于一个使用服务器集群来处理用户请求的站点来说是非常常见的。By default,
both Apache and IIS embed data in the ETag that dramatically reduces the odds of
the validity test succeeding on web sites with multiple servers. 

Apache 1.3和2.x中实体标签的格式是：\ *inode-size-timestamp*\ 虽然一个文件在多个
服务器上可能都存放在相同的目录、文件大小、权限和时间戳都相同，但是在每个服务器上
的inode几乎不可以相同。

IIS 5.0和6.0生成的实体标签也有类似的问题，IIS中实体标签的格式为：\
*Filetimestamp:ChangeNumber*\ 。其中\ *ChangeNumber*\ 用来追踪记录IIS配置文件的
变化次数。站点集群中的各IIS服务器的\ *ChangeNumber*\ 几乎不可能相同。

上面问题导致的结果就是：\ **不同服务器上的WEB服务器（Apache, IIS等）对同一个文件
生成的实体标签并不匹配**\ 如果实体标签不匹配，用户就不会收到响应数据少的304状态
码，而是收到状态码200和所有的请求数据。如果你的站点只是保存在一个服务器上，那么
这将不会对你产生影响。但是如果你的站点比较大，使用集群来提供服务，并且你使用的
Apache或IIS的默认ETag配置，你的用户访问页面将比较慢，而且服务器会处理高负载，同
时消耗更多的带宽，代理也不能有效的缓存你的站点内容。\ **即使你的内容组件使用一\
个“永不过期”的Expires头，当用户重新载入或刷新页面时，条件HTTP GET请求将会一直发\
起。**

如果你没有从实体标签提供的验证模型中受益，最好的做法是将其从HTTP响应头中移除。\
``Last-Modified``\ 头提供了基于文件时间戳的验证。从HTTP响应头中移除\ ``ETag``\
段可以减小HTTP响应头的大小和后续请求头的大小。此篇文章中介绍了\ `IIS服务器`_\ 如
果移除\ ``ETag``\ 。对于Apache服务器，只需要将下面的内容添加到配置文件中就可以了
。

.. sourcecode:: apache

    FileETag none

.. _IIS服务器:  http://support.microsoft.com/?id=922733

.. note::

    ``ETag``\ 值的生成与服务器有关，同一内容在不同服务器上生成的\ ``ETag``\ 值是
    不相同的，对于使用集群提供服务的站点必须注意这个问题。

    另外\ ``Expires``\ 和\ ``ETag``\ 头的优先级是什么样的？\ ``ETag``\ 优先于\
    ``Expires``\ ？


使得Ajax可缓存
==============
标签：内容

由于使用Ajax可以异步地向后台WEB服务器请求数据，使用Ajax的一个优势是可以向用户即\
时的提供反馈。然而使用Ajax并不能保证用户会等待JavaScript异步请求和返回XML数据。\
在大多数应用中，用户是否等待取决于如何使用Ajax。例如，在基于WEB的EMail客户端应\
用中，用户通常会等待Ajax返回所有符合他们搜索条件的结果。请铭记：\ ``“异步“并不\
意味着“瞬间”``\ 。

为了改进性能，优化Ajax响应（时间）至关重要。提高Ajax性能的最重要方法是使得Ajax\
的响应内容可缓存。正如前面讨论过的\ :ref:`添加一个Expires或Cache-Control HTTP头
<section_expires>`\ ，一些其它规则也可以用于Ajax：

*   :ref:`使用Gzip压缩页面内容 <section_gzip>`
*   :ref:`减少DNS查找次数 <section_dns>`
*   :ref:`减小JavaScript代码大小 <section_minify>`
*   :ref:`避免重定向 <section_redirect>`
*   :ref:`配置实体标签 <section_etags>`

让我们看一个例子，一个WEB 2.0的电子邮件客户端可能会使用Ajax下载用户的地址来实现\
自动实例。如果用户从上次使用电子邮件Wep APP后没有修改更新地址薄，且Ajax的响应内\
容通过\ ``Expires``\ 或\ ``Cache-Control``\ 进行了缓存，Ajax请求可以直接从缓存\
中读取数据。浏览器必须告知什么时候使用缓存，什么发起一个新的请求。这可以通过在\
通过 Ajax请求地址薄时的URL中添加一个时间戳，用于标示地址薄的最后修改时间，例如：
*&t=1190241612*\ 。如果地址薄自从上次下载之后没有发生变化，那么（请求时的）时间\
戳应该相同，并且可以从浏览器缓存中读取数据，这样就可以减少额外的HTTP请求操作；\
如果用户修改了她的地址薄，时间戳将保证新的URL不会匹配已缓存的响应内容，浏览器将\
会请求更新地址薄条目。

即使你的Ajax响应是动态创建的，并且只适用于单个用户，他们仍然可以被缓存，这样做\
可以使得你的WEB 2.0应用更快。


.. note::

    缓存Ajax内容

及早冲刷缓存
=============
标签：服务器

当用户请求一个页面时， it can take anywhere from 200 to 500ms for the backend
server to stitch together the HTML page. 在此期间，浏览器一直空闲着等待服务器返\
回数据。在PHP中，你可以使用函数\ ``flush()``\ 输出已加载的缓存。这样可以将已经\
读入的部分HTML数据发送给客户端，这样在后端服务器忙于加载HTML页面的其它部分时，\
浏览器就可以开始下载页面内容。The benefit is mainly seen on busy backends or
light frontends.

A good place to consider flushing is right after the HEAD because the HTML for
the head is usually easier to produce and it allows you to include any CSS and
JavaScript files for the browser to start fetching in parallel while the backend
is still processing.

例如：

.. sourcecode:: html

    ... <!-- css, js -->
    </head>
    <?php flush(); ?>
    <body>
    ... <!-- content -->

Yahoo搜索先锋研究和真实用户测试证明了使用这些技术是有好处的。


Ajax使用GET方法请求
===================
标签：服务器

Yahoo邮箱团队发现但使用XMLHttpRequest，浏览器内部通过两步来实现POST方法：先发送\
请求头，再发送数据。因此最好使用GET方法，因为GET方法只发送一个TCP包（除非你有大\
量的cookies）。IE中URL的最大长度为2K，因此如果你发送超过2K的数据时，就不能使用\
GET方法了。

有趣的是，POST方法没有像GET方法一样发送任何数据，基于\ `HTTP规范`_\ ，GET方法是\
用于获取信息的，当你只是请求数据，而没有发送数据存放在服务端时，你应该使用GET方\
法。

.. note::

    浏览器通过两步完成POST方法，至少发送两个TCP包，而GET方法通常只需发送一个TCP\
    包。（不同浏览器实现是否不同？）

    IE中URL最大长度为2K，即GET方法最多可发送2K的数据；那么其它浏览器呢？


.. _HTTP规范:   http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html


Post-load Components
=====================
标签：内容

仔细审视一下你的页面，问问自己”为了初始渲染页面，哪些东西是绝对必需的？”其它内容
和组件可以等一等。

JavaScript非常容易分为页面加载前和加载后两类。例如，你有一些用于拖放和动画的
JavaScript库，那么它们就可以等等再加载，因为拖放页面上的元素只能在页面初始渲染完
后才能进行。其它可能被暂缓加载的内容包括隐藏内容（需要用户动作才会显示的内容）和
折叠内容下的图片。

有一些工具可以帮助你实现这些工作：\ `YUI Image Loader`_\ 允许你延迟折叠内容下的
图片，\ `YUI Get utility`_\ 是一个包含JS和CSS的简单方法。例如，可以打开Firebug的
Net面板，再加载Yahoo主页看看。

(Post-load Components)与其它WEB开发最佳实践结合可以取得更好的效果。在这种情况下
，逐步增强的想法告诉我们，如果浏览器支持，JavaScript能够提高用户体验，但是你不得
不确保页面在没有JavaScript时也能正常工作。在你确认页面可以很好渲染后，你可以使用
一些post-load组件内容来为页面添加更新花哨的功能，如（元素的）拖放和动画等。

.. note::

    对页面元素建立一个加载优先顺序，逐步加载页面，以期尽快的向用户展示页面而提高
    用户体验。

.. _YUI Image Loader:   https://developer.yahoo.com/yui/imageloader/
.. _YUI Get utility:    https://developer.yahoo.com/yui/get/


预加载内容
===========
标签：内容

预加载(preload)看上去与延迟加载(post-load)是相对立的，但事实上它们有着不同的目标
，通过预加载你能够利用浏览器空闲时间来请求那些你将需要页面组件（如图片，样式表和
脚本）。当用户访问下一个页面时，这种方式（预加载）使得有大多数组件内容已经缓存在
客户端，那么用户加载页面时将会更快。

通常预加载被分为下面几种类型：

*   *无条件预加载(unconditional preload)*\ 加载完某个内容后，马上开始获取一些额
    外的内容组件。例如，可以查看一下google.com如何加载一个精灵(sprite)的？这个精
    灵图片并不是google.com主页所必须的，但是后续的搜索结果页面需要它。

*   *条件加载(conditional preload)*\ 基于用户动作（的分析）你可以猜想用户接下来
    将会做什么并对其进行预加载。在search.yahoo.com页面，你可以看到当你在输入框输
    入内容后，有哪儿额外的组件资源被请求加载了。

*   *期待预加载(anticipated preload)*\ 在进行重新设计前。完成重新设计后，你经常
    会听到“新站点很酷，但是加载速度比之前的慢”。部分原因是因为在用户访问时，已经
    缓存有旧站点的数据，但对于访问新站点，客户端并没有缓存。通过在重新设计之前，
    甚至是开始重新设计后，你可能通过（用户）访问旧站点时浏览器的空闲时，加载一些
    新站点会使用到的资源（如图片、脚本之类）来消除前面提到的重新设计带来的负面效
    应。


.. note::

    合理预加载——大数据分析，人工智能？


减少DOM元素数量
===============
标签：内容

一个复杂的网页意味着需要下载更多的数据，另外JavaScript读取DOM时更慢。例如，当你\
想增加一个事件处理时需要遍历500和5000个DOM时，两者非常的不同。

存在大量的DOM元素意味着可以通过删除页面中非必需元素加以改进。为了页面布局你是否\
使用过嵌套表格呢？你是否使用更多的\ ``div``\ 标签来改善这个问题呢？这可能是一种\
更好、（语义上）更加正确的方法改进你的HTML文件。

`YUI CSS工具`_\ 对页面布局有非常大的帮助：文件\ *grids.css*\ 可以帮助你进行页面\
布局；文件\ *fonts.css, reset.css*\ 可以帮助你去掉浏览器的默认格式设置。这让你\
可以重新开始思考你使用的标签（来布局），例如只有当语义上正常，而还是因为需要渲\
染一个新行而使用\ ``div``\ 标签。

DOM元素的数目很容易测算，在\ ``Firebug``\ 的命令栏输入：\
``document.getElementsByTagName('*').length``\ 就可以得到。

那么多少DOM元素算太多呢？检查对比其它标记（设计）良好页面。例如Yahoo主页，一个\
非常繁忙的页面，页面元素（HTML标签）数目一直少于700。


.. _YUI CSS工具:    https://developer.yahoo.com/yui/



将页面内容拆分到不同域
======================
标签：内容

通过拆分内容组件，你可以最大化并行下载。确保你使用的域名数没有超过4个，因为DNS\
查找也是有代价的。例如你可以将HTML文件和动态内容存放在域www.example.com中，将静\
态内容拆分到static1.example.com和static2.example.com下。

更多的信息请查看由Tenni Theurer和Patty Chi撰写的\ `通过拼车道最大化并行下载
<http://yuiblog.com/blog/2007/04/11/performance-research-part-4/>`_\ 。



最小化iframe数量
=================
标签：内容

框架（Iframes）允许HTML文档插入到其父文档中。只有理解了框架是如何工作的，才能更\
加有效的利用它。

<iframe> pros:

*   有助于较慢的第三方内容，如商标和广告
*   安全沙盒（没明白）
*   并行下载脚本（没明白）

<iframe> cons:

*   即使是空白的，也会消耗资源
*   阻止页面加载
*   Non-semantic


No 404s
========
标签：内容

HTTP请求要消耗相当的资源，因为发起一个HTTP请求最终得到一个无意义的响应是完全没\
有必要的，它不会给用户带来任何好处，且降低用户体验。

有些站点提供在404s提供一个提示信息”你的意思是X？“，这对用户有很大的帮助，但是会\
浪费服务端资源（如数据库等）。特别糟糕的是当链接一个外部JavaScript文件失败，返\
回404，这首先会阻塞并行下载，接下来浏览器可能尝试解析404响应返回的内容，试图从\
其中找到有用的东西。

.. note::

    外链JavaScript返回404时，浏览器会尝试解析404返回的内容。


减少Cookie大小
===============
标签：cookie

HTTP cookies有着各种用途，如用户的认证和定制。cookies的内容通过HTTP的头部在WEB\
服务器和浏览器之间进行交换。重要的是尽可能的减小cookies的大小，以降低它对用户响\
应时间的影响。

更多的信息请查看Tenni Theurer和Patty Chi撰写的“\ `When the Cookie Crumbles`_\ ”\
，其主要观点为：

*   删除非必需的cookies
*   保持cookies尽可能的小，以降低它对用户响应时间的影响
*   请在合适的域级别设置cookie，以免影响其它域名
*   （为cookie）设置一个合适的过期日期，在(cookie)过期前或被删除前，可以提高用\
    户响应时间

.. note::

    减小cookie的大小，以减小HTTP传输数据量，从而提高用户响应时间；为cookie设置\
    一个过期日期，将cookie缓存在客户端。

.. _When the Cookie Crumbles:
   http://yuiblog.com/blog/2007/03/01/performance-research-part-3/

Use Cookie-free Domains for Components
=======================================
标签：cookie

当浏览器请求一个静态文件（如图片）时附加上cookie信息，服务器并不会使用这些\
cookie。因此它们（cookie）只是浪费网络流量而毫无意义。\ **你应该确保在请求静态\
组件内容时不会携带cookie**\ 。可以创建一个子域用来存放静态内容。

假设你的域名为\ *www.example.org*\ 你可以将静态内容置于\ *static.example.org*\
下。然而，如果你将cookie设定在顶级域\ *example.org*\ 而不是\ *www.example.org*\
将导致在请求\ *static.example.org*\ 下的内容时也会携带cookie。在这种情况下，你\
可能需要使用一个全新的域名来存放静态内容，以保持此域\ *cookie-free*\ 。例如：\
Yahoo使用yimg.com，YouTube使用\ *ytimg.com*\ ，Amazon使用\ *images-amazon.com*\
来存放静态内容。

将静态内容存放在一个\ *cookie-free*\ 域的另一优势是：有些代理可以不会缓存包含\
cookie的请求。如此相关，如果你还没有想好使用\ *example.com*\ 还是\
*www.example.com*\ 作为你的主页，考虑到cookie的影响，如果没有\ *www*\ ，你毫无\
选择，只能在cookie的作用域中写\ *\*.example.org*\ ，出于（前面所提到的）性能上\
的因素，最好还是使用\ *www*\ 子域作用主页，并将cookie写在子域上。

.. note::

    竭力减少不必要的数据流量。对于不会使用cookie的静态内容，在请求时杜绝携带\
    cookie。

Minimize DOM Access
====================
标签：javascript

JavaScript存取DOM元素通常慢，为了得到更多响应内容，你可以：

*   对已存取过的元素进行缓存
*   更新那些离线的结点，并且他们加入到树中
*   避免通过JavaScript来固定侧面布局

更加的信息你可以查看Julien Lecomte撰写的“\ `高性能Ajax应用
<http://yuiblog.com/blog/2007/04/11/performance-research-part-4/>`\ ”。


Develop Smart Event Handlers
=============================
标签：javascript

有时页面响应较慢是因为DOM树中的不同元素被附加了太多的事件，而且这些事件被频繁的\
执行。如果你在div中有10个按钮，只添加一个事件处理程序在div上以代替为每个按钮添\
加一个事件处理程序，事件发生时，你可以捕捉到事件并知道是那个按钮产生的事件。这\
就是为什么使用\ *事件代理(event delegations)*\ 是一个好方法。

You also don't need to wait for the onload event in order to start doing
something with the DOM tree. Often all you need is the element you want to
access to be available in the tree. You don't have to wait for all images to be
downloaded. DOMContentLoaded is the event you might consider using instead of
onload, but until it's available in all browsers, you can use the YUI Event
utility, which has an onAvailable method

更多信息请查看Julien Lecomte所写的\ `高性能Ajax应用
<http://yuiblog.com/blog/2007/12/20/video-lecomte/>`_\ 。

.. _YUI Event:  https://developer.yahoo.com/yui/event/
.. _onAvailable:    https://developer.yahoo.com/yui/event/#onavailable

使用<link>代替@import
======================
标签：CSS

前面的规则已经建议将CSS放置在页面的顶部以允许浏览器进行逐步渲染。

对于IE, \ ``@import``\ 的作用与将\ ``<link>``\ 放置在页面底部等效，因此最好不要\
使用\ ``@import``\ 。


Avoid Filters
==============
标签：CSS

IE特有的过滤器\ ``AlphaImageLoader``\ 主要是为了修正低版本IE（<7）中真彩色PNG图\
片的半透明问题。这个过滤器会的问题在于：在图片在下载时，阻止浏览器进行渲染并会\
冻结浏览器；另外应该这一特性的元素会消耗更多的内存，不仅是图片，因此会引起更大\
的问题。

最好的做法是完全避免使用\ ``AlphaImageLoader``\ ，使用在IE中工作的很好的PNG8来\
代替，如果你不得不使用它，请使用\ ``_filter``\ 来避免影响到使用IE7+的用户。


优化图片大小
============
标签：图片

当设计师为你的网页制作好图片，在将图片上传至WEB服务器之前，你还可以对图片进行一\
些处理：

*   你可以检查GIF图片，看看使用的调色板大小是否与图片使用的色彩数相匹配。使用\
    ``imagemagick``\ 可以很容易完成这项工作你可以检查GIF图片，看看使用的调色板\
    大小是否与图片使用的色彩数相匹配。使用\ `imagemagick`_\ 很容易完成这个任务。
    当图片使用了四色和256色调色板时，图片大小就有很大的压缩空间。
*   试着将GIF转换为PNG格式，看看图片大小是否会减小，通常会减小一些。由于（以前\
    ）浏览器对PNG图片的支持有限，不过这已经成为过去时了。（在此过程中）唯一的问\
    题是真彩色PNG中的alpha透明，但是GIF不支持真彩色，也不支持可变的透明度，因为\
    GIF 可以做到的，PNG（PNG8）调色板都可以做到（除了动画之外）。\
    ``imagemagick``\ 中只需要输入下面的命令就可以安全的将GIF转换为PNG：

    .. sourcecode:: text

        convert image.gif image.png

    我们只需要说：给PNG一个机会。

*   使用PNG优化工具优化你的PNG图片，如\ `pngcrush`_\ ：
    
    ..sourcecode:: text

        pngcrush image.png -rem alla -reduce-brute result.png

*   使用JPEGs优化工具\ *jpegtran*\ 优化你的JPEG图片。它可以对JPEG进行一些无损操\
    作，如：旋转，另外也可以优化和删除图片中的注释和其它无用信息（如EXIF信息）。

    .. sourcecode:: text

        jpegtran -copy none -optimize -perfect src.jpeg dest.jpeg

.. _imagemagick: http://www.imagemagick.org/
.. _pngcrush:   http://pmt.sourceforge.net/pngcrush/


优化CSS Sprites
================
标签：图片

*   从水平方向布置图片通常比在垂直方向布置得到的图片更小；
*   将颜色类似的图片合并到一个sprite中，可以降低颜色数量，理想情况是使得颜色小\
    于256种可以符合PNG8标准
*   为了使用sprite移动友好，图片间间隔不要太大。这并不会影响文件的大小，但是用\
    户浏览器只需要较少的内存将图片解压为像素图。如：100×100的图片需要10000个像\
    素；1000×1000的图片需要一百万个像素。

不要在HTML中缩放图片
====================
标签：图片

不要因为可以通过HTML标签中的\ ``width``\ 和\ ``height``\ 属性来调整图片大小而使\
用超过你需要的图片。如果你需要\ ``<img width="100" height="100" src="mycat.jpg"
alt="My Cat" />``\ ,那么你的图片尺寸应该是100x100px，而不是使用规格为500x500px\
进行缩放。


使用favicon.ico尽可能小且可缓存
===============================
标签：图片

*favicon.ico*\ 是存放在你的站点顶级路径下的一张图片。它是一个无法避免的evil，因\
为即使你不关心，浏览器还是会一直请求它，因此最好不要返回一个\ *404 Not Found*\
的响应。由于（与其它内容）在同一服务器上，所以每次请求\ *favicon.ico*\ 都会附带\
上cookie。这个图片也会影响到下载顺序。例如在IE中，当请求加载某一个组件时，\
*favicon.ico*\ 会在被请求内容之前下载。

因此为了减轻\ *favicon.ico*\ 带来的不良影响，请确保：

*   *favicon.ico*\ 文件比较小，最小小于1K
*   通过\ ``Expires``\ 为\ *favicon.ico*\ 设置一个合适的过期时间（如果你想更新\
    favicon.ico，无法重命名）。你可以安全的过期日期设定为几个月之后。你可以能检\
    查当前favicon.ico的修改时间以作出明智的决定。

.. note::

    浏览器在什么情况下会去请求\ *favicon.ico*\ ？

`Imagemagick`_\ 可以帮助你创建更小的favicon图标。


.. _Imagemagick:    http://www.imagemagick.org/


使页面组件小于25K
=================
标签：移动

这个限制主要是因为iPhone不缓存大于25K的文件内容。需要注意的是这个大小是指未压缩\
时的大小。This is where minification is important because gzip alone may not be
sufficient.

Pack Components into a Multipart Document
==========================================
标签：移动

将组件合并到一个multipart document就像一封附带附件的电子邮件，它可以帮助你使用\
一个HTTP请求来获取多个组件内容（请记住：HTTP请求是非常昂贵的）。当你使用这项技\
术时，首先需要检查用户浏览器是否支持此技术。


Avoid Empty Image src
======================
标签：服务端

下面两种情况可能导致一个\ *img*\ 标签的\ *src*\ 属性为空:

*   HTML文档中：\ ``<img src=''>``
*   JavaScript：

    .. sourcecode:: javascript

        var img = new Image();
        img.src = '';

上面两种情况都会导致浏览器额外的请求：

*   IE将对页面所在的目录发起请求
*   Safari和Chrome (WebKit)会请求实际页面
*   Firefox 3及之前的版本的行为与WebKit相同，3.5以后的版本解决了这个\ `问题
    <https://bugzilla.mozilla.org/show_bug.cgi?id=444931>`_\ ，不再发送请求
*   Opera遇到上面的情况则什么也不会做（即不会发出额外的请求）

为什么这是一种糟糕的做法？

1.  向服务器发送大量无用的请求(unexpected traffic)会降低服务器的性能，特别是那\
    些日浏览量达到百万的站点（攻击服务器方法之一？没听过）
2.  浪费服务器的CPU去生成一个永远不会被访问的页面
3.  可能会对用户数据造成误导。如果你正在通过cookie或其它方法追踪请求状态，你有\
    可能破坏数据。即使是请求一张不会返回结果的图片，相应的响应头已经被浏览器读\
    取接受，其中包括cookie，而响应的其它部分则被丢弃了，这可能会造成损害。（没\
    太明白原文的意思）

导致这一问题的根源是浏览器进行URI解析的方式。在\ :rfc:`3986`\ ——统一资源标识\
（Uniform Resource Identifiers, URI）对此进行了定义。当URI是一个空字符串时，它\
被看作是一个相对URI，并且依据5.2部分算法进行解析。对于特殊情况——空字符串，5.4\
部分有介绍。Firefox, Safari和Chrome都按照正确的规范解决了URI为空字符串的问题，\
而IE则没有正确的处理这个问题，虽然表面上看IE依据了早期的规范\ :rfc:`2396`\ （但\
是它已经被\ :rfc:`3986`\ 所取代）。从技术上讲，所有的浏览器都做了它们应该什么的\
事情，但问题是，造成URI为空完全是无心之过。

HTML5在4.8.2节增加了对标签的\ *src*\ 的说明，告诉浏览器不要进行额外的请求：

|   *src*\ 属性必须存在，且必须包含一个有效的URL，这个URL可以指向一个非交互式的\
|   ，可以为动画的图片资源，但是不能是页面或脚本。如果此元素URI的base部分与文档\
|   相同，那么\ *src*\ 的值不得为空字符串。

但愿浏览器将来不会在存在这个问题，不幸的是，对\ *<script src=''>*\ 和\ *<link
href=''*\ 。也许还有时间进行调整确保浏览器不会意外的声明这种行为。

这条规则是Yahoo的JavaScript专家Nicolas C. Zakas所发现。更详细的信息请查看他的文\
章“\ `空的img的src值可以摧毁你的站点
<http://www.nczonline.net/blog/2009/11/30/empty-image-src-can-destroy-your-site
/>`_\ ”。



参考资料
=========
.. [#ref]   `Best Practices for Speeding Up Your Web Site <https://developer.yahoo.com/performance/rules.html>`_
