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

# classify by ratio (only exist in SIFT) 也可以用fundamentalMatrix
matches=[m for m,n in matches if m.distance < 0.99*n.distance]

#key points from left and right image
list_key_point_matched_left,\
list_key_point_matched_right=C_F_M.KeyPointsFromMatches(list_key_point_original_left,
                                                        list_key_point_original_right,
                                                        matches)

#coordinate transformation
list_point_matched_left=np.array([this_key_point.pt for this_key_point in list_key_point_matched_left])
list_point_matched_right=np.array([this_key_point.pt for this_key_point in list_key_point_matched_right])

M, mask = cv2.findHomography(list_point_matched_left, list_point_matched_right, cv2.RANSAC, 5.0)

h, w = image_left.shape[:2]

# 使用得到的变换矩阵对原图像的四个角进行变换,获得在目标图像上对应的坐标。
# 输入原始图像和变换之后的图像的对应4个点，便可以得到变换矩阵。
# 之后用求解得到的矩阵输入perspectiveTransform便可以对一组点进行变换：
pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)


# 注意这里src和dst的输入并不是图像，而是图像对应的坐标
dst = cv2.perspectiveTransform(pts, M)
#cv2.findHomography().如果我们传了两个图像里的点集合，
# 它会找到那个目标的透视转换。然后我们可以使用cv2.perspectiveTransform()来找目标，
# 它需要至少4个正确的点来找变换。

 #得到旋转矩阵getPerspectiveTransform函数：根据输入和输出点获得图像透视变换的矩阵
#getPerspectiveTransform()：计算4个二维点对之间的透射变换矩阵 H（3行x3列）
perspectiveM = cv2.getPerspectiveTransform(np.float32(dst), pts)
# perspectiveM[2][1] = 0
# perspectiveM[2][0] = 0

# estimateRigidTransform()：计算多个二维点对或者图像之间的最优仿射变换矩阵（2行x3列），H可以是部分自由度，比如各向一致的切变。
# getAffineTransform()：计算3个二维点对之间的仿射变换矩阵H（2行x3列），自由度为6.
# warpAffine()：对输入图像进行仿射变换
# findHomography：计算多个二维点对之间的最优单映射变换矩阵H（3行x3列） ，使用最小均方误差或者RANSAC方法 。
# getPerspectiveTransform()：计算4个二维点对之间的透射变换矩阵H（3行x3列）
# warpPerspective()：对输入图像进行透射变换
# perspectiveTransform()：对二维或者三维矢量进行透射变换，也就是对输入二维坐标点或者三维坐标点进行投射变换。
# estimateAffine3D：计算多个三维点对之间的最优三维仿射变换矩阵H （3行x4列）
# transform()：对输入的N维矢量进行变换，可用于进行仿射变换、图像色彩变换.
# findFundamentalMat：计算多个点对之间的基矩阵H。
#warpPerspective()：对输入图像进行透射变换

image_right_warped = cv2.warpPerspective(image_right, perspectiveM, (w, h))
image_right_warped_croped= cv2.warpPerspective(image_right, M, (w, h))

plt.figure(figsize=(13,6))
plt.imshow(C_F_M.bgr_rgb(image_left))
plt.xticks([])
plt.yticks([])
plt.title('image left',fontdict=title_prop)

plt.figure(figsize=(13,6))
plt.imshow(C_F_M.bgr_rgb(image_right))
plt.xticks([])
plt.yticks([])
plt.title('image right',fontdict=title_prop)

plt.figure(figsize=(13,6))
plt.imshow(C_F_M.bgr_rgb(image_right_warped))
plt.xticks([])
plt.yticks([])
plt.title('image right warped',fontdict=title_prop)

plt.figure(figsize=(13,6))
plt.imshow(C_F_M.bgr_rgb(image_right_warped_croped))
plt.xticks([])
plt.yticks([])
plt.title('image right warped-croped',fontdict=title_prop)

# //利用基础矩阵剔除误匹配点
# vector<uchar> RansacStatus;
# Mat Fundamental= findFundamentalMat(p01,p02,RansacStatus,FM_RANSAC);
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
