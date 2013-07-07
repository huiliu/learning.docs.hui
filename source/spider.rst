抓取环境保护部的空气质量数据
******************************

计划
=====
* 利用\ `scrapy`_\ 框架来建立一个爬虫抓取数据
* 将数据存入到MySQL中
* 对空气质量进行分析，统计

.. _scrapy: http://scrapy.org

操作
=====
按照网页中表格格式在MySQL中建立数据表：

.. code-block:: sql

   CREATE TABLE airq (id AUTO_INCREMENT, cityname CHAR(25), date DATE, api INTEGER, pp CHAR(25), aql CHAR(5), aqs CHAR(5)


