# -*- coding: utf-8 -*-
"""
Created on Fri Nov 13 11:41:03 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-SIFT Matching
"""

from __init__ import *

# load image
folder_path='../Material/'

image_left=cv2.imread(folder_path+'L22.jpg')
image_right=cv2.imread(folder_path+'R22.jpg')

#load image
# image_left=cv2.imread(folder_path+'\\L13.jpg')
# image_right=cv2.imread(folder_path+'\\R13.jpg')

# image_left=cv2.imread('top_VCM_390.png')
# image_right=cv2.imread('top_VCM_400.png')

print("SIFT detector......")

n_key_points=1000

detector=cv2.xfeatures2d.SIFT_create(n_key_points)
    
# find the keypoints and descriptors with SIFT
list_key_point_original_left,descriptor_left=detector.detectAndCompute(image_left,None)
list_key_point_original_right,descriptor_right=detector.detectAndCompute(image_right,None)

"""
Brute Force匹配和FLANN匹配是opencv二维特征点匹配常见的两种办法，分别对应BFMatcher和FlannBasedMatcher。

二者的区别：

    BFMatcher总是尝试所有可能的匹配，从而使得它总能够找到最佳匹配。

    FlannBasedMatcher中FLANN的含义是Fast Library forApproximate Nearest Neighbors，它是一种

近似法，算法更快但是找到的是最近邻近似匹配，当我们需要找到一个相对好的匹配但是不需要最佳匹配

的时候可以用FlannBasedMatcher。当然也可以通过调整FlannBasedMatcher的参数来提高匹配的精度或者

提高算法速度，但是相应地算法速度或者算法精度会受到影响。
"""

# Brute Force Matcher with default params
matcher=cv2.FlannBasedMatcher()
# matcher=cv2.BFMatcher()

matches=matcher.knnMatch(descriptor_left,descriptor_right,k=2)

#classify by ratio (only exist in SIFT) 也可以用fundamentalMatrix
matches=[m for m,n in matches if m.distance < 0.99*n.distance]

#matched key points
list_key_point_matched_left=[list_key_point_original_left[this_match.queryIdx] for this_match in matches]
list_key_point_matched_right=[list_key_point_original_right[this_match.trainIdx] for this_match in matches]

#list of point (pt)
list_point_left=[this_key_point.pt for this_key_point in list_key_point_matched_left]
list_point_right=[this_key_point.pt for this_key_point in list_key_point_matched_right]

#通过RANSAC消除误匹配，并用RANSAC_status记录是否剔除
# M, RANSACStatus = cv2.findHomography(list_point_matched_left, list_point_matched_right, cv2.RANSAC, 5.0)
fundamental_matrix, RANSAC_status=cv2.findFundamentalMat(np.array(list_point_left),
                                                          np.array(list_point_right),
                                                          cv2.RANSAC);

#define good matches
good_matches=[]

for k in range(len(RANSAC_status)):
    
    if RANSAC_status[k]:
    
        good_matches.append(matches[k])

#key points from good matches
list_good_key_point_left,\
list_good_key_point_right=C_F_M.KeyPointsFromMatches(list_key_point_original_left,
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
C_F_M.DrawMatchedLines(list_good_key_point_left,
                       list_good_key_point_right,
                       image_left)

#draw matching result points
C_F_M.DrawMatchedPoints(image_left,
                        list_good_key_point_left,
                        list_good_key_point_right)

x_shift=C_F_M.CalculateHorizontalDifference(list_good_key_point_left,
                                            list_good_key_point_right)

y_shift=C_F_M.CalculateVerticalDifference(list_good_key_point_left,
                                          list_good_key_point_right)

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

