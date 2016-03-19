#!/bin/env python2
# -*- encoding:utf-8 -*-

#
# Author: Liu Hui
# Date: Thu Mar 14 23:21:42 CST 2013
#
# 利用matplotlib图形库来处理数据作图。需要安装matplotlib包。
#
# 此处将作图的一些配置文件写为json的格式导入，希望可以做到数据，程序，控制分离
#

import json
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import spline
from scipy import interpolate

f = open('config.json','r')
config = json.load(f)
f.close()

data = np.genfromtxt(config['inputfile']['name'],
                        delimiter=config['inputfile']['delimiter']).transpose()
newx = np.linspace(data[0].min(), data[0].max(), 500)
#for kind in ['nearest', 'zero', 'slinear', 'quadratic', 'cubic']:
for kind in ['cubic']:
    f = interpolate.interp1d(data[0], data[7], kind=kind)
    newy = f(newx)
    plt.plot(newx, newy, label=kind)
    plt.plot(data[0], data[7], '+')
plt.title(config['title'])
plt.xlabel(config['xlabel'])
plt.ylabel(config['ylabel'])
plt.xlim(config['xrange'])
plt.ylim(config['yrange'])
plt.legend(loc=config['legend']['location'])
plt.savefig(config['outputfile']['name'], dpi=config['outputfile']['dpi'])
