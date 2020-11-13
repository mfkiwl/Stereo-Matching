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

# load image
folder_path=r'D:\GitHub\KAMERAWERK\Binocular-Stereo-Matching\matlab\Material'

image_left=cv2.imread(folder_path+'\\L13.jpg')#绝对路径
image_right=cv2.imread(folder_path+'\\R13.jpg')#绝对路径

# image_a=cv2.imread('top_VCM_390.png')
# image_b=cv2.imread('top_VCM_400.png')

n_key_points=None

# 初始化ORB
ORB = cv2.ORB_create(n_key_points)

# 寻找关键点并计算描述符
key_points_left, descriptor_left = ORB.detectAndCompute(image_left,None)
key_points_right, descriptor_right = ORB.detectAndCompute(image_right,None)

# # 画出关键点
# outimg1 = cv2.drawKeypoints(image_left,keypoints=kp1,outImage=None)
# outimg2 = cv2.drawKeypoints(image_right,keypoints=kp2,outImage=None)
	
# 示关键点
# outimg3 = np.hstack([outimg1, outimg2])
# cv2.imshow("Key Points", outimg3)
# cv2.waitKey(0)

# 初始化 BFMatcher
bf = cv2.BFMatcher(cv2.NORM_HAMMING)

# 对描述子进行匹配
matches = bf.match(descriptor_left, descriptor_right)

#key points from left and right image
key_points_left,key_points_right=C_F_M.KeyPointsFromMatches(key_points_left,
                                                            key_points_right,
                                                            matches)

#slope of each key point in the list
slope_key_points=C_F_M.SlopeFromKeyPoints(key_points_left,key_points_right,image_left)

#good matches based on slope
good_matches=C_F_M.CalculateGoodMatches(slope_key_points,
                                        matches)

# C_R.RANSAC(range(len(slope_key_points)),slope_key_points)

#key points from good matches
good_key_points_left,\
good_key_points_right=C_F_M.KeyPointsFromMatches(key_points_left,
                                                 key_points_right,
                                                 good_matches)
plt.figure()

#draw matching result dual images
C_F_M.DrawDualImages(image_left,image_right)

#draw matching result lines
C_F_M.DrawMatchedLines(good_key_points_left,
                       good_key_points_right,
                       image_left)

#draw matching result points
C_F_M.DrawMatchedPoints(image_left,
                        good_key_points_left,
                        good_key_points_right)

y_shift=C_F_M.CalculateVerticalDifference(good_key_points_left,
                                          good_key_points_right)

print(y_shift)

plt.title(str(y_shift))
