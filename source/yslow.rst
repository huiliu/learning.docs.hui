Best Practices for Speeding Up Your Web Site
********************************************

https://developer.yahoo.com/performance/rules.html

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
