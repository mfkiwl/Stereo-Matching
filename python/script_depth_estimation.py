# -*- coding: utf-8 -*-
"""
Created on Fri Nov 13 20:35:12 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-depth estimation

"""

from __init__ import *


# load image
folder_path='../Material/'

#gray level
# image_left=cv2.imread(folder_path+'L13.jpg')
# image_right=cv2.imread(folder_path+'R13.jpg')

image_left=cv2.imread(folder_path+'L14.png',0)
image_right=cv2.imread(folder_path+'R14.png',0)

for window_size in [1,3,5,7,9,11,13,15,17,19][:2]:
    
    # SGBM Parameters -----------------
    window_size = 3                     # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
     
    # stereo = cv2.StereoBM_create(numDisparities=16, blockSize=15)
    stereo = cv2.StereoSGBM_create(minDisparity=0,
                                   numDisparities=160,             # max_disp has to be dividable by 16 f. E. HH 192, 256
                                   blockSize=5,
                                   P1=8 * 3 * window_size ** 2,    # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
                                   P2=32 * 3 * window_size ** 2,
                                   disp12MaxDiff=-1,
                                   uniquenessRatio=15,
                                   speckleWindowSize=5,
                                   speckleRange=2,
                                   preFilterCap=63,
                                   mode=cv2.STEREO_SGBM_MODE_SGBM_3WAY)
    
    disparity_left=stereo.compute(image_left,image_right)
    disparity_right=stereo.compute(image_right,image_left)
    
    plt.figure(figsize=(17,6))
    
    plt.subplot(121)
    plt.imshow(disparity_left,'rainbow')
    
    plt.subplot(122)
    plt.imshow(disparity_right,'terrain')
    
plt.figure()
plt.imshow(image_left,'gray')
