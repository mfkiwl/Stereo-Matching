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

# SGBM Parameters -----------------
window_size = 3                     # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
 
# stereo = cv2.StereoBM_create(numDisparities=16, blockSize=15)
stereo = cv2.StereoSGBM_create(minDisparity=0,
    numDisparities=160,             # max_disp has to be dividable by 16 f. E. HH 192, 256
    blockSize=5,
    P1=8 * 3 * window_size ** 2,    # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
    P2=32 * 3 * window_size ** 2,
    disp12MaxDiff=1,
    uniquenessRatio=15,
    speckleWindowSize=0,
    speckleRange=2,
    preFilterCap=63,
    mode=cv2.STEREO_SGBM_MODE_SGBM_3WAY)

disparity = stereo.compute(image_left,image_right)

kernel_size=13

# disparity_blurred=cv2.blur(disparity,(kernel_size,kernel_size),0)
# disparity_blurred=cv2.GaussianBlur(disparity,(kernel_size,kernel_size),0)

# plt.figure()
# plt.imshow(image_left,'gray')

plt.figure()
plt.imshow(disparity[50:-50,50:-50],'gray')

import numpy as np
from sklearn.preprocessing import normalize
import cv2
 
print('loading images...')
imgL = cv2.imread(folder_path+'L14.png')  # downscale images for faster processing
imgR = cv2.imread(folder_path+'R14.png')
 
# SGBM Parameters -----------------
window_size = 3                     # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
 
left_matcher = cv2.StereoSGBM_create(
    minDisparity=0,
    numDisparities=160,             # max_disp has to be dividable by 16 f. E. HH 192, 256
    blockSize=5,
    P1=8 * 3 * window_size ** 2,    # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
    P2=32 * 3 * window_size ** 2,
    disp12MaxDiff=1,
    uniquenessRatio=15,
    speckleWindowSize=0,
    speckleRange=2,
    preFilterCap=63,
    mode=cv2.STEREO_SGBM_MODE_SGBM_3WAY
)
 
right_matcher = cv2.ximgproc.createRightMatcher(left_matcher)

print('computing disparity...')
displ = left_matcher.compute(imgL, imgR)  # .astype(np.float32)/16
dispr = right_matcher.compute(imgR, imgL)  # .astype(np.float32)/16
displ = np.int16(displ)
dispr = np.int16(dispr)

plt.figure()
plt.imshow(displ[50:-50,50:-50],'gray')

plt.figure()
plt.imshow(dispr[50:-50,50:-50],'gray')
