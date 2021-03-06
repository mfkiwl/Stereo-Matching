# -*- coding: utf-8 -*-
"""
Created on Mon Nov 23 18:27:28 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-read y-shift data and plot
"""

from __init__ import *
                               
file_path='../Outcome/y-shift_20201113_143105.txt'

file=open(file_path,"r")   #设置文件对象

data=file.readlines()  #直接将文件中按行读到list里，效果与方法2一样

file.close()  #关闭文件

#str2num
list_y_shift=[float(this_str) for this_str in data]

plt.figure(figsize=(13,6))

plt.ylim([-30,30])
plt.plot(np.array(range(len(list_y_shift)))/1500,
         list_y_shift,
         'k.',
         markersize=0.6)

plt.ylabel('Y-Shift (pixel)',fontdict=label_prop)
plt.xlabel('Time (min)',fontdict=label_prop)

plt.title('Y-Shift Distribution from Frames in Video Streaming',fontdict=title_prop)

#set ticks fonts
plt.tick_params(labelsize=12)
labels=plt.gca().get_xticklabels()+plt.gca().get_yticklabels()

#label fonts
[this_label.set_fontname('Times New Roman') for this_label in labels]

plt.savefig('../Outcome/y-shift.png',dpi=300,bbox_inches='tight')