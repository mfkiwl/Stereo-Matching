# -*- coding: utf-8 -*-
"""
Created on Mon Nov  9 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title：script-ORB Matching
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt

import calculation_ransac as C_R

def bgr_rgb(img):
    
    (r, g, b) = cv2.split(img)
    return cv2.merge([b, g, r])

def draw_match(img1, img2, kp1, kp2, match):
    
    outimage = cv2.drawMatches(img1, kp1, img2, kp2, match, outImg=None)
    plt.imshow(bgr_rgb(outimage))

# load image
folder_path=r'D:\GitHub\KAMERAWERK\Binocular-Stereo-Matching\matlab\Material'

image_a = cv2.imread(folder_path+'\\L3.bmp')#绝对路径
image_b = cv2.imread(folder_path+'\\R3.bmp')#绝对路径

# image_a=cv2.imread('top_VCM_390.png')
# image_b=cv2.imread('top_VCM_400.png')

# 初始化ORB
orb = cv2.ORB_create()

# 寻找关键点
kp1 = orb.detect(image_a)
kp2 = orb.detect(image_b)

# 计算描述符
kp1, des1 = orb.compute(image_a, kp1)
kp2, des2 = orb.compute(image_b, kp2)

# 画出关键点
outimg1 = cv2.drawKeypoints(image_a, keypoints=kp1, outImage=None)
outimg2 = cv2.drawKeypoints(image_b, keypoints=kp2, outImage=None)
	
# 示关键点
# outimg3 = np.hstack([outimg1, outimg2])
# cv2.imshow("Key Points", outimg3)
# cv2.waitKey(0)

# 初始化 BFMatcher
bf = cv2.BFMatcher(cv2.NORM_HAMMING)

# 对描述子进行匹配
matches = bf.match(des1, des2)

#calculate slope of key points
slope_key_points=[]

for this_match in matches:
    
    #obtain key points from matching result
    point_left=np.array(kp1[this_match.queryIdx].pt)
    point_right=np.array(kp2[this_match.trainIdx].pt)
    
    this_slope=(point_right-point_left)[1]/np.shape(image_a)[1]
    
    slope_key_points.append(this_slope)
    
# 计算最大距离和最小距离
min_distance = matches[0].distance
max_distance = matches[0].distance

for x in matches:
    
    if x.distance < min_distance:
        
        min_distance = x.distance
        
    if x.distance > max_distance:
        
        max_distance = x.distance

# 筛选匹配点
'''
当描述子之间的距离大于两倍的最小距离时，认为匹配有误。
但有时候最小距离会非常小，所以设置一个经验值30作为下限。
'''
good_match = []

for x in matches:
    
    if x.distance <= max(2 * min_distance, 100):
        
        good_match.append(x)

# 绘制匹配结果
draw_match(image_a, image_b, kp1, kp2, good_match)

list_query_index=[]
list_train_index=[]

for this_match in good_match:
    
    list_query_index.append(this_match.queryIdx)
    list_train_index.append(this_match.trainIdx)

#draw good match on left image
for this_index in list_query_index:
    
    plt.plot(kp1[this_index].pt[0],kp1[this_index].pt[1],'r.')
    
#draw good match on right image
for this_index in list_train_index:
    
    plt.plot(kp2[this_index].pt[0]+np.shape(image_a)[1],kp2[this_index].pt[1],'b.')
    
plt.figure()

slope_threshold=0.01

for k in range(len(slope_key_points)):
    
    this_slope=slope_key_points[k]
    
    if abs(this_slope)<slope_threshold:
        
        plt.plot(k,this_slope,'k.')
        
    else:
            
        plt.plot(k,this_slope,'g.')
        
# C_R.RANSAC(range(len(slope_key_points)),slope_key_points)