# -*- coding: utf-8 -*-
"""
Created on Wed Mar 24 11:13:29 2021

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: script-Stereo Effect Experiment
"""

from __init__ import *

folder_name_a=r'E:\GitHub\KAMERAWERK\VCM-Dual\Material\Stereo Effect Experiment\A'
folder_name_b=r'E:\GitHub\KAMERAWERK\VCM-Dual\Material\Stereo Effect Experiment\B'

# image_name='VCM_300.png'

# #read image
# image_left=cv2.imread(folder_name_a+'//'+image_name)
# image_right=cv2.imread(folder_name_b+'//'+image_name)

str_window_name='SBS'

# cv2.namedWindow(str_window_name, cv2.WINDOW_NORMAL)

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
    
    WIDTH_BLANKED=int(WIDTH*16/9)
    
    i_start=int(0.5*(WIDTH_BLANKED-WIDTH))
    
    #define sbs format image
    image_merged=np.zeros((HEIGHT,WIDTH_BLANKED*2,3),dtype=np.uint8)
    
    #add blank
    image_merged[:HEIGHT,i_start:i_start+WIDTH,:]=image_left
    image_merged[:HEIGHT,i_start+WIDTH_BLANKED:i_start+WIDTH_BLANKED+WIDTH,:]=image_right_warped
    
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
        
    cv2.imwrite('../Outcome/'+this_image_name,image_merged)
