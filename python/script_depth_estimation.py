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

stereo = cv2.StereoSGBM_create()
# stereo = cv2.StereoBM_create()

disparity = stereo.compute(image_left,image_right)

kernel_size=13

disparity_blurred=cv2.blur(disparity,(kernel_size,kernel_size),0)
# disparity_blurred=cv2.GaussianBlur(disparity,(kernel_size,kernel_size),0)

plt.figure()
plt.imshow(image_left,'gray')

plt.figure()
plt.imshow(disparity_blurred,'gist_rainbow')
