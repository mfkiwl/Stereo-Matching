# -*- coding: utf-8 -*-
"""
Created on Mon Nov  9 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-ORB Matching
"""

from __init__ import *

# load image
folder_path='../Material/'

image_left=cv2.imread(folder_path+'L13.jpg')
image_right=cv2.imread(folder_path+'R13.jpg')

# image_left=cv2.imread('top_VCM_390.png')
# image_right=cv2.imread('top_VCM_400.png')

# image_left=cv2.imread(folder_path+'L3.bmp')
# image_right=cv2.imread(folder_path+'R3.bmp')

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

plt.savefig('slope.png',dpi=300,bbox_inches='tight')

# C_R.RANSAC(range(len(slope_key_points)),slope_key_points)

#key points from good matches
good_key_points_left,\
good_key_points_right=C_F_M.KeyPointsFromMatches(key_points_original_left,
                                                 key_points_original_right,
                                                 good_matches)

'''original matching'''
plt.figure(figsize=(17,6))

#draw matching result dual images
C_F_M.DrawDualImages(image_left,
                     image_right)

#draw matching result lines
C_F_M.DrawMatchedLines(key_points_original_left,
                       key_points_original_right,
                       image_left)

#draw matching result points
C_F_M.DrawMatchedPoints(image_left,
                        key_points_original_left,
                        key_points_original_right)

plt.xticks([])
plt.yticks([])

plt.title('Feature Matching (original)',fontdict=title_prop)

plt.savefig('original matching.png',dpi=300,bbox_inches='tight')

'''good matching'''
plt.figure(figsize=(17,6))

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

y_shift_final=int(np.round(y_shift/4)*4)

if x_shift>0:
    
    print('-- Conclusion: Image A is left, and image B is right.')

if x_shift<0:
    
    print('-- Conclusion: Image A is right, and image B is left.')

if y_shift>0:
    
    print_str='Result: Right image is %d (+%.4f in reality) pixels higher than left one'%(y_shift_final,y_shift)

if y_shift<0:
    
    print_str='Result: Left image is %d (-%.4f in reality) pixels higher than right one'%(y_shift_final,y_shift)
    
plt.xticks([])
plt.yticks([])

plt.title('Feature Matching (good)',fontdict=title_prop)

plt.savefig('good matching.png',dpi=300,bbox_inches='tight')

print('-- Result:',print_str)