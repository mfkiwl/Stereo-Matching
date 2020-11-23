# -*- coding: utf-8 -*-
"""
Created on Wed Nov 18 17:16:03 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-read slope data and plot
"""

from __init__ import *

file_path=r'D:\GitHub\KAMERAWERK\Binocular-Stereo-Matching\cpp\slope.txt'

file=open(file_path,"r")   #设置文件对象

data=file.readlines()  #直接将文件中按行读到list里，效果与方法2一样

file.close()  #关闭文件

#str2num
slope_key_points=[float(this_str) for this_str in data]

#max slope
slope_threshold=0.6
n_interval=200

#list to collect distance sum for each slope 
list_distance_for_each_slope=[]

#select a suitable slop from this list
list_slope_candidate=list(np.linspace(-slope_threshold,slope_threshold,n_interval))

#find the best slope
for this_slope_candidate in list_slope_candidate:
    
    this_distance=0
    
    for this_slope in slope_key_points:
        
        this_distance+=np.abs(this_slope-this_slope_candidate)

    list_distance_for_each_slope.append(this_distance)
    
#minimum of the sum of the distance between candidate and real slope counts
index_min=list_distance_for_each_slope.index(np.min(list_distance_for_each_slope))
   
#suitable slope from matching result
matched_slope=list_slope_candidate[index_min]

print('==> matched slope:',matched_slope)

plt.figure(figsize=(13,6))

#list good matched result
good_matches=[]

#traverse all slopes and make classification
for k in range(len(slope_key_points)):
    
    this_slope=slope_key_points[k]
    
    #the key points whose slope is near matched slop may be considered
    if abs(this_slope-matched_slope)<slope_threshold/n_interval:

        plt.plot(k,this_slope,'k.')
        
    else:
            
        plt.plot(k,this_slope,'g.')
        
plt.ylim([-slope_threshold,slope_threshold])

# plt.hlines(-0.021,0,len(slope_key_points),'r')

plt.ylabel('y-shift (pixel)',fontdict=label_prop)
plt.xlabel('frame (-)',fontdict=label_prop)

#set ticks fonts
plt.tick_params(labelsize=12)
labels=plt.gca().get_xticklabels()+plt.gca().get_yticklabels()

#label fonts
[this_label.set_fontname('Times New Roman') for this_label in labels]