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

def DrawMatch(img1, img2, kp1, kp2, match):
    
    outimage = cv2.drawMatches(img1, kp1, img2, kp2, match, outImg=None)
    plt.imshow(bgr_rgb(outimage))

def DrawMatchedLines(key_points_left,key_points_right,image_left):
    
    for k in range(len(key_points_left)):
        
        plt.plot([key_points_left[k][0],key_points_right[k][0]+np.shape(image_left)[1]],
                 [key_points_left[k][1],key_points_right[k][1]],
                 '-',
                 linewidth=.13)
        
def MergeDualImages(image_left,image_right):
    
    if np.shape(image_left)!=np.shape(image_right):
        
        print('=> ERROR: Incorrect Dimension of Dual Images')
        
        return
    
    image_merged=np.zeros((np.shape(image_left)[0],
                           np.shape(image_left)[1]*2,
                           3))
    
    #left image
    image_merged[:,:np.shape(image_left)[1],:]=image_left[:,:,:]
    image_merged[:,np.shape(image_left)[1]:np.shape(image_left)[1]*2,:]=image_right[:,:,:]
          
    return image_merged.astype(image_left[0,0,0])

def DrawDualImages(image_left,image_right):
    
    plt.imshow(bgr_rgb(MergeDualImages(image_left,image_right)))
    
def KeyPointsFromMatches(matches):
    
    key_points_left=[]
    key_points_right=[]
    
    for this_match in matches:
        
        #obtain key points from matching result
        key_points_left.append(np.array(kp1[this_match.queryIdx].pt))
        key_points_right.append(np.array(kp2[this_match.trainIdx].pt))
 
    return key_points_left,key_points_right

def SlopeFromKeyPoints(key_points_left,key_points_right,image_left):
    
    slope_key_points=[]
    
    for k in range(len(key_points_left)):
        
        point_left=key_points_left[k]
        point_right=key_points_right[k]
        
        diff_x=np.shape(image_left)[1]-(point_left-point_right)[0]
        diff_y=(point_right-point_left)[1]
    
        #calculate slope of key points
        this_slope=diff_y/diff_x
            
        slope_key_points.append(this_slope)
    
    return slope_key_points 

def DrawMatchedPoints(image_left,kp1,kp2,matches):

    list_query_index=[]
    list_train_index=[]
    
    for this_match in matches:
        
        list_query_index.append(this_match.queryIdx)
        list_train_index.append(this_match.trainIdx)
    
    #draw good match on left image
    for this_index in list_query_index:
        
        plt.plot(kp1[this_index].pt[0],
                 kp1[this_index].pt[1],
                 'r.',
                 markersize=2.3)
        
    #draw good match on right image
    for this_index in list_train_index:
        
        plt.plot(kp2[this_index].pt[0]+np.shape(image_left)[1],
                 kp2[this_index].pt[1],
                 'b.',
                 markersize=2.3)
        
# load image
folder_path=r'D:\GitHub\KAMERAWERK\Binocular-Stereo-Matching\matlab\Material'

image_left=cv2.imread(folder_path+'\\L13.jpg')#绝对路径
image_right=cv2.imread(folder_path+'\\R13.jpg')#绝对路径

# image_a=cv2.imread('top_VCM_390.png')
# image_b=cv2.imread('top_VCM_400.png')

# 初始化ORB
orb = cv2.ORB_create()

# 寻找关键点
kp1 = orb.detect(image_left)
kp2 = orb.detect(image_right)

# 计算描述符
kp1, des1 = orb.compute(image_left,kp1)
kp2, des2 = orb.compute(image_right,kp2)

# 画出关键点
outimg1 = cv2.drawKeypoints(image_left,keypoints=kp1,outImage=None)
outimg2 = cv2.drawKeypoints(image_right,keypoints=kp2,outImage=None)
	
# 示关键点
# outimg3 = np.hstack([outimg1, outimg2])
# cv2.imshow("Key Points", outimg3)
# cv2.waitKey(0)

# 初始化 BFMatcher
bf = cv2.BFMatcher(cv2.NORM_HAMMING)

# 对描述子进行匹配
matches = bf.match(des1, des2)

#key points from left and right image
key_points_left,key_points_right=KeyPointsFromMatches(matches)

#slope of each key point in the list
slope_key_points=SlopeFromKeyPoints(key_points_left,key_points_right,image_left)

#max slope
slope_threshold=0.02
n_interval=10

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
    
index_min=list_distance_for_each_slope.index(np.min(list_distance_for_each_slope))

#suitable slope from matching result
matched_slope=list_slope_candidate[index_min]

plt.figure()

good_matches=[]

#the key points whose slope is near matched slop may be considered
for k in range(len(slope_key_points)):
    
    this_slope=slope_key_points[k]

    if abs(this_slope-matched_slope)<slope_threshold/n_interval:

        good_matches.append(matches[k])
        
        plt.plot(k,this_slope,'k.')
        
    else:
            
        plt.plot(k,this_slope,'g.')
        
# C_R.RANSAC(range(len(slope_key_points)),slope_key_points)

#key points from good matches
good_key_points_left,good_key_points_right=KeyPointsFromMatches(good_matches)

#y coordinate of key points
y_good_key_points_left=[this_key_point[1] for this_key_point in good_key_points_left]
y_good_key_points_right=[this_key_point[1] for this_key_point in good_key_points_right]

y_shift=np.average(np.array(y_good_key_points_left)-np.array(y_good_key_points_right))

plt.figure()

#draw matching result dual images
DrawDualImages(image_left,image_right)

#draw matching result lines
DrawMatchedLines(good_key_points_left,good_key_points_right,image_left)

#draw matching result points
DrawMatchedPoints(image_left,kp1,kp2,good_matches)

print(y_shift)

plt.title(str(y_shift))
