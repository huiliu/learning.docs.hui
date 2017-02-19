爬虫框架 - scrapy
**********************


.. author:: default
.. categories:: python
.. tags:: program, python, spider
.. comments::
.. more::



架构图
======

.. image:: images/scrapy_architecture.png

数据流向说明
-------------

``scrapy``\ 引擎控制着整个爬虫的运行，引导数据到不同组件之间交流。\
大概步骤如下：\ [#]_ [#]_

1.  ``scrapy``\ 引擎打开一个域名，同时分配爬虫来处理这个域名，并且告诉爬虫从第\
    一个URL开始工作；
2.  The Engine gets the first URLs to crawl from the Spider and schedules them\
    in the Scheduler, as Requests.
3.  ``scrapy``\ 引擎向调度器请求下一个URL
4.  调度器返回下一个URL给\ ``scrapy``\ 引擎，\ ``scrapy``\ 引擎通过下载中间件\
    将其发送给下载器
5.  当一个页面下载完成，下载器会生成一个Response，并通过下载中间件将Response回传给\ ``scrapy``\ 引擎。（response direction）
6.  ``scrapy``\ 引擎收到来自下载器的Response后，通过爬虫中间件将其发送给爬虫进行处理；
7.  爬虫处理完Response的数据后返回一个scraped项和新的Request给\ ``scrapy``\ 引擎;
8.  ``scrapy``\ 引擎将scraped项交给Pipeline进一步处理，将新的Request发送给调度器；
9.  从第二步开始重复，直到调度器中的所有请求均被处理，\ ``scrapy``\ 引擎将关闭当前域名。

.. todo::

    scrapy scheduler在哪里定义的？
    下载中间件的作用是什么？如何工作的？
    爬虫在哪里，如何响应Response对象的？

XPath语法
===========
XPath 是一门在 XML 文档中查找信息的语言。XPath 用于在 XML 文档中通过元素和属性\
进行导航。\ [#]_

.. todo::

    准备加入XPath的一些资料和使用心得


抓取环境保护部的空气质量数据
==============================

计划
-----
* 利用\ `scrapy`_\ 框架来建立一个爬虫抓取数据
* 将数据存入到MySQL中
* 对空气质量进行分析，统计

.. _scrapy: http://scrapy.org

操作
-----
按照网页中表格格式在MySQL中建立数据表：

.. code-block:: sql

   CREATE TABLE airq (id AUTO_INCREMENT, cityname CHAR(25), date DATE, api INTEGER, pp CHAR(25), aql CHAR(5), aqs CHAR(5)


参考资料
========
.. [#]  http://doc.scrapy.org/en/0.18/topics/architecture.html#data-flow
.. [#]  http://www.magentonotes.com/python-scrapy-architecture.html
.. [#]  http://www.w3school.com.cn/xpath/xpath_intro.asp
