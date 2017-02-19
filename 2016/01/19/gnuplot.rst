Introduce to the GNUPlot
*************************

本文通过学习\ **马欢**\ [#ref1]_\ 的Gnuplot系列教程后,练习处理我的计算数据完成。


.. author:: default
.. categories:: learn
.. tags:: learn, gnuplot, software
.. comments::
.. more::



文中代码均可在\ **GNUPlot Shell**\ 中运行完成。

使用帮助
========
安装好\ `GnuPlot`_\ 后，运行\ ``gnuplot``\ 即进入命令模式，可以使用命令\
``help``\ 查看帮助信息:

.. _GnuPlot: http://en.wikipedia.org/wiki/Gnuplot

.. sourcecode:: bash

    gnuplot> help

会出现一段说明文字，操作与用less查看文件一样。用'\ **q**\ '退出后会提示相关\
Topic，与绘制2D图形相关的是"**plotting**"，输入"**plotting**"即可看到相关的帮助\
信息，同样，退出后，你会看到更多的Command或Topic信息，输入相应项就可以查看帮助\
了。如果想要返回上一级帮助主题，连续再次回车即可，不过不会再出现对应的信息列表\
了（比较麻烦，也有可以是我还不熟悉）。

.. sourcecode:: bash

    Help topic:plotting

绘图命令\ ``plot``
====================
GNUPlot的功能强大，控制灵活。目前我只用到\ ``plot``\ 命令以绘制一些数据。下面给\
出一些例子：

绘制一个简单的函数
-------------------
绘制\ *sin(x)*\ 曲线：

.. sourcecode:: gnuplot

    # 文件模式。也可以直接在GNUPlot Shell中运行
    plot sin(x)

执行上面的命令后，会弹出绘制好\ *sin(x)*\ 曲线的窗口。X轴方向范围为\[-10,10\]，\
这个默认范围。

在图形的右上角有一个碍眼的图例（害的我作点状图时想了好久，图上为什么多了一个点\
）。如果图上有好几条曲线，图例就是必须的了。gnuplot提供的相应的设定参数来控制它\
的行为：

.. sourcecode:: text

    # 关闭图例
    unset key  
    # 关闭默认图例title
    set noautotitle
    # 开启图例外框，使用默认的边框线型和颜色
    set box
    # 设定图例所在位置，如右下
    set right bottom
    # 其它更多设置请查看帮助
    help set
    # Subtopic of set: key

从文件中读取数据并绘制
----------------------
``plot``\ 命令可以从\ `CSV`_\ 格式文 件中直接读取数据并作图。数据文件summary如下：

.. _CSV: http://en.wikipedia.org/wiki/Comma-separated_values

| 1.3    -0.03    -6.15
| 1.4    -4.72    -10.22
| 1.5    -7.26    -12.16
| 1.6    -8.60    -12.70
| 1.8    -8.34    -11.74
| 2.0    -6.99    -9.55
| 2.2    -5.26    -7.35
| 2.5    -3.33    -4.58
| 2.8    -2.11    -2.75
| 3.1    -1.22    -1.69
| 3.5    -0.51    -0.86
| 4.0    -0.13    -0.31
| 4.5    -0.09    -0.12
| 5.0    -0.07    -0.08
| 6.0    -0.04    -0.06
| 7.0    -0.03    -0.04
| 8.0    -0.02    -0.03
| 9.0    -0.02    -0.03
| 10.0   -0.01    -0.02 

执行命令：

.. sourcecode:: gnuplot

    plot "summary" using 1:2

上面的命令即：利用文件"summary"的第一列作为\ *x*\ , 第二列作为*y*作图，默认为点\
状图，可以利用选项\ ``with lines``\ 来作出线状图，如：

.. sourcecode:: gnuplot

    plot "summary" using 1:2 with lines

可是\ ``gnuplot``\ 就会简单的将图上的点用直线连接起来，图形极可能不光滑。

绘制光滑的曲线
--------------
如果你有一系列坐标，如果用\ ``gnuplot``\ 直接绘制，曲线是不光滑的，数学上有一系\
列方法根据已知数据来拟合光滑曲线的方法，如插值法，外推法等，\ ``gnuplot``\ 也提\
供一些简单的功能来光滑我们的曲线。

利用\ ``plot``\ 命令中的"``smooth``"属性来设定一些参数就可以实现，如：

.. sourcecode:: gnuplot

    plot "summary" using 1:2 smooth csplines

你会发现你的图形已经变得光滑，关于"``smooth``"的详细参数，请查看帮助信息

设定坐标轴信息
--------------
自定义坐标的范围，标签等

.. sourcecode:: gnuplot

    set xrange [1:10.5]
    set yrange [-13:0.2]
    set xlabel "X label"
    set ylabel "Y label"
    # 其它一些更详细的设定，如：刻度间隔等请查看帮助

其它对标签还有其它很多属性,如字体等：

.. sourcecode:: gnuplot

    set xlabel "X label" font "Monospace,16"

将x轴的标签文本字体设定为"*Monospace*", 大小为16

输出到文件
==========
最终我们希望将图形输出到文件，可能是图片，latex等格式，\ ``gnuplot``\ 一概可以\
搞定。

设定字符集
----------
首先设定一下字符集，推荐使用\ **utf8**\ ，它可以兼容所有字符。

.. sourcecode:: gnuplot

    set encoding utf8

设定输出文件格式
------------------
``gnuplot``\ 支持N多种输出文件格式，如图形(png, jpg, tiff, emf等）, latex,
epslatex等，有好些我都不知道。下面的命令将输出emf矢量图片:

.. sourcecode:: gnuplot

    # enhanced属性允许你在标签中使用类latex格式的文本
    set term emf enhanced
    set output "summary.emf"
    plot 'summary' using 1:2 linewidth 2 smooth csplines
    set output

运行完后你会发现当前目录下生成了一个"*summary.emf*"图形文件

参考资料
=========

.. [#ref1]  http://blog.sciencenet.cn/blog-373392-535918.html
