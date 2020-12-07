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

plt.figure(figsize=(17,23))

count=1

 # wsize default 3; 5; 7 for SGBM reduced size image; 15 for SGBM full size image (1300px and above); 5 Works nicely
for window_size in [1,3,5,7,9,11,13,15]:
    
    # SGBM Parameters -----------------
    
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
    
    disparity=stereo.compute(image_left,image_right)
    
    plt.subplot(4,2,count)
    
    plt.imshow(disparity,'rainbow')
    plt.xticks([])
    plt.yticks([])
    
    plt.title('window size: %d'%window_size,fontdict=title_prop)
    
    count+=1
    
plt.savefig('window size.png',dpi=300,bbox_inches='tight')    

plt.figure(figsize=(13,6))
plt.imshow(image_left,'gray')

plt.xticks([])
plt.yticks([])

plt.title('original image',fontdict=title_prop)
