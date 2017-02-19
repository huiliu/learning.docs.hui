Apache日志过滤－是否可以用来预防DDOS
**************************************

.. author:: default
.. categories:: program, c/c++
.. tags:: c/c++
.. comments::
.. more::


最近公司的用户中心登陆入口一直被人尝试登陆，查看日志可以发现是通过字典暴力尝试\
。查看\ `Apache`\ 的文档发现其日志可以使用管道进行处理。遂想是否可以通过在\
`Apache`\ 的日志输出加一个过滤程序对日志进行实时处理分析，如果某段时间内来自某\
个IP的访问量超过设定阀值就将其拉入小黑屋关上十天半个月。

写了一个小程序测试了一个还可以：https://github.com/huiliu/ipf.httpd.network.hui 
