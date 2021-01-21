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

image_left=cv2.imread(folder_path+'L15.jpg')
image_right=cv2.imread(folder_path+'R15.jpg')

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

# classify by ratio (only exist in SIFT)
matches=[m for m,n in matches if m.distance < 0.99*n.distance]

#key points from left and right image
list_key_point_matched_left,\
list_key_point_matched_right=C_F_M.KeyPointsFromMatches(list_key_point_original_left,
                                                    list_key_point_original_right,
                                                    matches)

#coordinate transformation
list_point_matched_left=[for this_key_point in list_key_point_matched_left]
list_point_matched_right=[for this_key_point in list_key_point_matched_right]

vector<Point2f>p01,p02;
for (size_t i=0;i<matches.size();i++)
{
    p01.push_back(R_keypoint01[i].pt);
    p02.push_back(R_keypoint02[i].pt);
}

//利用基础矩阵剔除误匹配点
vector<uchar> RansacStatus;
Mat Fundamental= findFundamentalMat(p01,p02,RansacStatus,FM_RANSAC);
# # Apply ratio test
# good = [[m] for m, n in matches if m.distance < 0.5 * n.distance]

# # cv2.drawMatchesKnn expects list of lists as matches.
# img3 = cv2.drawMatchesKnn(image_left, 
#                           key_points_left,
#                           image_right,
#                           key_points_right,
#                           good,
#                           None,
#                           flags=2)

# plt.figure(figsize=(17,6))
# plt.imshow(C_F_M.bgr_rgb(img3))

# #key points from left and right image
# key_points_left,\
# key_points_right=C_F_M.KeyPointsFromMatches(key_points_left,
#                                             key_points_right,
#                                             matches)

# #slope of each key point in the list
# slope_key_points=C_F_M.SlopeFromKeyPoints(key_points_left,key_points_right,image_left)

# #good matches based on slope
# good_matches=C_F_M.CalculateGoodMatches(slope_key_points,
#                                         matches)

# # C_R.RANSAC(range(len(slope_key_points)),slope_key_points)

# plt.figure()

# #draw matching result dual images
# C_F_M.DrawDualImages(image_left,
#                      image_right)

# #key points from good matches
# good_key_points_left,\
# good_key_points_right=C_F_M.KeyPointsFromMatches(good_matches)

# #draw matching result lines
# C_F_M.DrawMatchedLines(good_key_points_left,
#                        good_key_points_right,
#                        image_left)

# #draw matching result points
# C_F_M.DrawMatchedPoints(image_left,
#                         good_key_points_left,
#                         good_key_points_right)

# y_shift=C_F_M.CalculateVerticalDifference(good_key_points_left,
#                                           good_key_points_right)
