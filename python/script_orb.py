# -*- coding: utf-8 -*-
"""
Created on Mon Nov  9 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-ORB Matching
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt

import calculation_ransac as C_R

import calculation_feature_matching as C_F_M

title_prop={'family':'Gill Sans MT',
            'weight':'normal',
            'style':'normal',
            'size':18}

# load image
folder_path=r'D:\GitHub\KAMERAWERK\Binocular-Stereo-Matching\matlab\Material'

image_left=cv2.imread(folder_path+'\\L12.jpg')
image_right=cv2.imread(folder_path+'\\R12.jpg')

# image_left=cv2.imread('top_VCM_390.png')
# image_right=cv2.imread('top_VCM_400.png')

# image_left=cv2.imread(folder_path+'\\L3.bmp')
# image_right=cv2.imread(folder_path+'\\R3.bmp')

n_key_points=1000

# 初始化detector
detector=cv2.ORB_create(n_key_points)

# 寻找关键点并计算描述符
key_points_original_left,descriptor_left=detector.detectAndCompute(image_left,None)
key_points_original_right,descriptor_right=detector.detectAndCompute(image_right,None)

# 初始化 BFMatcher
matcher=cv2.BFMatcher(cv2.NORM_HAMMING)

# 对描述子进行匹配
matches=matcher.match(descriptor_left, 
                      descriptor_right)

#key points from left and right image
key_points_matched_left,\
key_points_matched_right=C_F_M.KeyPointsFromMatches(key_points_original_left,
                                                    key_points_original_right,
                                                    matches)

#slope of each key point in the list
slope_key_points=C_F_M.SlopeFromKeyPoints(key_points_matched_left,
                                          key_points_matched_right,
                                          image_left)

#good matches based on slope
good_matches=C_F_M.CalculateGoodMatches(slope_key_points,
                                        matches)

# C_R.RANSAC(range(len(slope_key_points)),slope_key_points)

#key points from good matches
good_key_points_left,\
good_key_points_right=C_F_M.KeyPointsFromMatches(key_points_original_left,
                                                 key_points_original_right,
                                                 good_matches)
plt.figure()

#draw matching result dual images
C_F_M.DrawDualImages(image_left,
                     image_right)

#draw matching result lines
C_F_M.DrawMatchedLines(good_key_points_left,
                       good_key_points_right,
                       image_left)

#draw matching result points
C_F_M.DrawMatchedPoints(image_left,
                        good_key_points_left,
                        good_key_points_right)

x_shift=C_F_M.CalculateHorizontalDifference(good_key_points_left,
                                            good_key_points_right)

y_shift=C_F_M.CalculateVerticalDifference(good_key_points_left,
                                          good_key_points_right)

print('==> x shift:',x_shift)
print('==> y shift:',y_shift)

y_shift_final=np.round(y_shift/4)*4

if x_shift>0:
    
    print('-- Conclusion: The 1st image is left, the 2nd image is right.')

if x_shift<0:
    
    print('-- Conclusion: The 1st image is right, the 2nd image is left.')

if y_shift>0:
    
    title_str='Result: Right image is %d (+%.4f in reality) pixels higher than left one'%(y_shift_final,y_shift)

if y_shift<0:
    
    title_str='Result: Left image is %d (-%.4f in reality) pixels higher than right one'%(y_shift_final,y_shift)
    
plt.xticks([])
plt.yticks([])

plt.title(title_str,fontdict=title_prop)

print('-- Result:',title_str)