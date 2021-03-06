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

image_left=cv2.imread(folder_path+'L22.jpg')
image_right=cv2.imread(folder_path+'R22.jpg')

# image_left=cv2.imread(folder_path+'L14.png')
# image_right=cv2.imread(folder_path+'R14.png')

# image_left=cv2.imread('input/top_VCM_390.png')
# image_right=cv2.imread('input/top_VCM_400.png')

# image_left=cv2.imread(folder_path+'L3.bmp')
# image_right=cv2.imread(folder_path+'R3.bmp')

n_key_points=1000

# 初始化detector
detector=cv2.ORB_create(n_key_points)

# 寻找关键点并计算描述符
list_key_point_original_left,descriptor_left=detector.detectAndCompute(image_left,None)
list_key_point_original_right,descriptor_right=detector.detectAndCompute(image_right,None)

# 初始化 BFMatcher
matcher=cv2.BFMatcher(cv2.NORM_HAMMING)

# 对描述子进行匹配
matches=matcher.match(descriptor_left, 
                      descriptor_right)

#key points from left and right image
list_key_point_matched_left,\
list_key_point_matched_right=C_F_M.KeyPointsFromMatches(list_key_point_original_left,
                                                    list_key_point_original_right,
                                                    matches)
#matched key points
list_key_point_matched_left=[list_key_point_original_left[this_match.queryIdx] for this_match in matches]
list_key_point_matched_right=[list_key_point_original_right[this_match.trainIdx] for this_match in matches]

#slope of each key point in the list
slope_key_points=C_F_M.SlopeFromKeyPoints(list_key_point_matched_left,
                                          list_key_point_matched_right,
                                          image_left)

# '''slope'''
# #good matches based on slope
# good_matches=C_F_M.GoodMatchesFromSlope(slope_key_points,matches)

# plt.savefig('../Outcome/slope.png',dpi=300,bbox_inches='tight')

'''ransac'''
#good matches based on ransac
good_matches,fundamental_matrix=C_F_M.GoodMatchesFromRANSAC(list_key_point_original_left,
                                                             list_key_point_original_right,
                                                             matches)

# C_R.RANSAC(range(len(slope_key_points)),slope_key_points)

#key points from good matches
list_key_point_good_left,\
list_key_point_good_right=C_F_M.KeyPointsFromMatches(list_key_point_original_left,
                                                     list_key_point_original_right,
                                                     good_matches)

'''original matching'''
plt.figure(figsize=(17,6))

#draw matching result dual images
C_F_M.DrawDualImages(image_left,
                     image_right)

#draw matching result lines
C_F_M.DrawMatchedLines(list_key_point_matched_left,
                       list_key_point_matched_right,
                       image_left)

#draw matching result points
C_F_M.DrawMatchedPoints(image_left,
                        list_key_point_matched_left,
                        list_key_point_matched_right)

plt.xticks([])
plt.yticks([])

plt.title('Feature Matching (original)',fontdict=title_prop)

plt.savefig('../Outcome/original matching.png',dpi=300,bbox_inches='tight')

'''good matching'''
plt.figure(figsize=(17,6))

#draw matching result dual images
C_F_M.DrawDualImages(image_left,
                     image_right)

#draw matching result lines
C_F_M.DrawMatchedLines(list_key_point_good_left,
                       list_key_point_good_right,
                       image_left)

#draw matching result points
C_F_M.DrawMatchedPoints(image_left,
                        list_key_point_good_left,
                        list_key_point_good_right)

x_shift=C_F_M.CalculateHorizontalDifference(list_key_point_good_left,
                                            list_key_point_good_right)

y_shift=C_F_M.CalculateVerticalDifference(list_key_point_good_left,
                                          list_key_point_good_right)

print('==> x shift:',x_shift)
print('==> y shift:',y_shift)

y_shift_final=int(np.round(y_shift/4)*4)

if x_shift>0:
    
    print('-- Conclusion: Image A is left, and image B is right.')

if x_shift<0:
    
    print('-- Conclusion: Image A is right, and image B is left.')

if y_shift>0:
    
    print_str='Result: Right image is %d (+%.4f in reality) pixels higher than left one'%(abs(y_shift_final),abs(y_shift))

if y_shift<0:
    
    print_str='Result: Left image is %d (-%.4f in reality) pixels higher than right one'%(abs(y_shift_final),abs(y_shift))
    
plt.xticks([])
plt.yticks([])

plt.title('Feature Matching (good)',fontdict=title_prop)

plt.savefig('../Outcome/good matching.png',dpi=300,bbox_inches='tight')

print('-- Result:',print_str)
