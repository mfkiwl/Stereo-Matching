# -*- coding: utf-8 -*-
"""
Created on Wed Mar 24 11:13:29 2021

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: script-Stereo Effect Experiment
"""

from __init__ import *

folder_name=r'E:\GitHub\KAMERAWERK\VCM-Dual\Material\Stereo Effect Experiment\20210326'
folder_name_a=folder_name+'\A'
folder_name_b=folder_name+'\B'

# image_name='VCM_300.png'

# #read image
# image_left=cv2.imread(folder_name_a+'//'+image_name)
# image_right=cv2.imread(folder_name_b+'//'+image_name)

str_window_name='SBS'

# cv2.namedWindow(str_window_name, cv2.WINDOW_NORMAL)

#define a list to collect merged image
list_image_merged=[]

for this_image_name in os.listdir(folder_name_a):
    
    #read image
    image_left=cv2.imread(folder_name_a+'//'+this_image_name)
    image_right=cv2.imread(folder_name_b+'//'+this_image_name)

    n_key_points=1000
    
    detector=cv2.xfeatures2d.SIFT_create(n_key_points)
        
    # find the keypoints and descriptors with SIFT
    list_key_point_original_left,descriptor_left=detector.detectAndCompute(image_left,None)
    list_key_point_original_right,descriptor_right=detector.detectAndCompute(image_right,None)
    
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
    
    #calculate homography matrix (通过SIFT特征点寻找的，浮点型坐标)
    homography_matrix, mask = cv2.findHomography(list_point_matched_left, list_point_matched_right, cv2.RANSAC, 5.0)
    
    HEIGHT, WIDTH = image_left.shape[:2]
    
    #define
    points_original = np.float32([[0, 0], [0, HEIGHT - 1], [WIDTH - 1, HEIGHT - 1], [WIDTH - 1, 0]]).reshape(-1, 1, 2)
    
    # 注意这里src和dst的输入并不是图像，而是图像对应的坐标（将所有像素点变换得到的坐标）
    points_transformed = cv2.perspectiveTransform(points_original, homography_matrix)
    
    #这里是反变换
    perspective_matrix = cv2.getPerspectiveTransform(np.float32(points_transformed), points_original)
    # perspective_matrix[2][1] = 0
    # perspective_matrix[2][0] = 0
    
    image_right_warped= cv2.warpPerspective(image_right, perspective_matrix , (WIDTH, HEIGHT))
    # image_right_warped= cv2.warpPerspective(image_right, homography_matrix , (WIDTH, HEIGHT))
    
    # plt.figure(figsize=(6,6))
    # plt.imshow(C_F_M.bgr_rgb(image_left))
    # plt.xticks([])
    # plt.yticks([])
    # plt.title('image left',fontdict=title_prop)
    
    # plt.figure(figsize=(6,6))
    # plt.imshow(C_F_M.bgr_rgb(image_right))
    # plt.xticks([])
    # plt.yticks([])
    # plt.title('image right',fontdict=title_prop)
    
    # plt.figure(figsize=(6,6))
    # plt.imshow(C_F_M.bgr_rgb(image_right_warped))
    # plt.xticks([])
    # plt.yticks([])
    # plt.title('image right homography',fontdict=title_prop)
    
    WIDTH_BLANKED=int(HEIGHT*16/9)
    
    i_start=int(0.5*(WIDTH_BLANKED-WIDTH))
    
    #define sbs format image
    image_merged=np.zeros((HEIGHT,WIDTH_BLANKED*2,3),dtype=np.uint8)
    
    #add blank
    image_merged[:HEIGHT,i_start:i_start+WIDTH,:]=image_left
    image_merged[:HEIGHT,i_start+WIDTH_BLANKED:i_start+WIDTH_BLANKED+WIDTH,:]=image_right
    
    # plt.figure(figsize=(13,6))
    # plt.imshow(C_F_M.bgr_rgb(image_merged))
    # plt.xticks([])
    # plt.yticks([])

    # plt.close()
    
    image_merged=cv2.resize(image_merged,(WIDTH_BLANKED,HEIGHT),cv2.INTER_AREA)
    
    # cv2.imshow(str_window_name,image_merged)
    # cv2.setWindowProperty(str_window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    
    # # 按下esc时，退出
    # cv2.waitKey(666)
        
    cv2.imwrite('../Outcome/20210326/'+this_image_name,image_merged)


# # 使用得到的变换矩阵对原图像的四个角进行变换,获得在目标图像上对应的坐标。
# # 输入原始图像和变换之后的图像的对应4个点，便可以得到变换矩阵。
# # 之后用求解得到的矩阵输入perspectiveTransform便可以对一组点进行变换：
# pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)


# # 注意这里src和dst的输入并不是图像，而是图像对应的坐标
# dst = cv2.perspectiveTransform(pts, M)
# #cv2.findHomography().如果我们传了两个图像里的点集合，
# # 它会找到那个目标的透视转换。然后我们可以使用cv2.perspectiveTransform()来找目标，
# # 它需要至少4个正确的点来找变换。

#  #得到旋转矩阵getPerspectiveTransform函数：根据输入和输出点获得图像透视变换的矩阵
# #getPerspectiveTransform()：计算4个二维点对之间的透射变换矩阵 H（3行x3列）
# perspectiveM = cv2.getPerspectiveTransform(np.float32(dst), pts)
# # perspectiveM[2][1] = 0
# # perspectiveM[2][0] = 0

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