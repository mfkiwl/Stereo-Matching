# -*- coding: utf-8 -*-
"""
Created on Mon Nov 23 18:27:28 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-read y-shift data and plot
"""

import numpy as np
import matplotlib.pyplot as plt

file_path=r'D:\GitHub\KAMERAWERK\Binocular-Stereo-Matching\cpp\y-shift.txt'

file=open(file_path,"r")   #设置文件对象

data=file.readlines()  #直接将文件中按行读到list里，效果与方法2一样

file.close()  #关闭文件

#str2num
list_y_shift=[float(this_str) for this_str in data]

# plt.ylim([-10,0])
plt.plot(range(len(list_y_shift)),list_y_shift,'k-')