#!/bin/env python2
# -*- encoding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import spline
from scipy import interpolate

plt.figure(figsize=(8,4), dpi=300)
#plt.figure(dpi=300)
dataFile = 'summary'
data = np.genfromtxt(dataFile, delimiter=',').transpose()
newx = np.linspace(data[0].min(), data[0].max(), 500)
#for kind in ['nearest', 'zero', 'slinear', 'quadratic', 'cubic']:
for kind in ['cubic']:
    f = interpolate.interp1d(data[0], data[7], kind=kind)
    newy = f(newx)
    plt.plot(newx, newy, label=kind)
    plt.plot(data[0], data[7], '+')

plt.title('The Interaction Energy Between $NH_4$...$OH_2$')
plt.xlabel('d/A')
plt.ylabel('E/$kcal.mol^{-1}$')
plt.xlim([1, 10.5])
plt.ylim([-9, 0.2])
plt.legend(loc='lower right')
plt.savefig('test1.png', dpi=120)
