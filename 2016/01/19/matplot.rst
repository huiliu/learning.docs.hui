Learning Matplot Library
************************
`Matplot`_\ 是一个超强的Python绘图库。

.. _Matplot: http://matplotlib.org/index.html

在此记录学习\ **Matplot**\ 一些例子，以便于日后可以较快的熟悉学习。


.. author:: default
.. categories:: learn
.. tags:: learn, matplot, software, python
.. comments::
.. more::



在此以量化计算的一些数据为例，如下将EDA计算解离过程的数据保存为一个CSV格式的数\
据文件，第一列为横坐标，其它列作为纵坐标。

.. literalinclude:: _static/code/python/matplot/summary
    :language: c

用于绘图的python代码如下,此部分的重点在如何拟合光滑的曲线。

直接法
======
这部分我将绘图的相关信息直接写入在python代码中。

.. literalinclude:: _static/code/python/matplot/smooth.py
    :language: python

对其中的函数进行简单的简介：

* ``numpy.genfromtxt``  从文件中读取数据
* ``numpy.linspace``    在某一数据段产生一系列均匀的数据。用于后面拟合滑曲线。
* ``scipy.interpolate.interp1d``    使用指定算法对数据进行拟合，得到一个函数。
* ``pyplot.figure`` 用于设定画布相关属性
* ``matplotlib.pyplot.plot``    绘制图形
* ``matplotlib.pyplot.title``   设定图形的标题
* ``matplotlib.pyplot.xlabel, ylabel``  分别设定横坐标、纵坐标的标签
* ``matplotlib.pyplot.xlim, ylim``      分别设定横坐标、纵坐标的取值范围
* ``matplotlib.pyplot.legend``          设定关于图例的一些属性
* ``matplotlib.pyplot.savefig``         输出图形至文件

使用json作配置文件
==================
这里我将绘图的一些配置信息以配置文件的方式存放，绘制图形时从配置文件中读取数据\
。最初愿望是\ **实现数据，程序，配置三者分离**\ ，便于通用的处理大量数据。配置\
文件为json格式，如下：

.. literalinclude:: _static/code/python/matplot/config.json
    :language: json

绘图代码略有修改，如下:

.. literalinclude:: _static/code/python/matplot/sm.py
    :language: python
