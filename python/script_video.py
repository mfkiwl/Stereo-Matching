# -*- coding: utf-8 -*-
"""
Created on Mon Nov 23 11:47:26 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-load video
"""

import cv2

folder_path=r'D:\工作\工业相机3D视频素材'

video_path=folder_path+'\20201113_143105.avi'

capture = cv2.VideoCapture(video_path)

"""
capture.get(0)   视频文件的当前位置（播放）以毫秒为单位
capture.get(1)   基于以0开始的被捕获或解码的帧索引
capture.get(2)   视频文件的相对位置（播放）：0=电影开始，1=影片的结尾。
capture.get(3)   在视频流的帧的宽度
capture.get(4)   在视频流的帧的高度
capture.get(5)   帧速率
capture.get(6)   编解码的4字-字符代码
capture.get(7)   视频文件中的帧数
capture.get(8)   返回对象的格式
capture.get(9)   返回后端特定的值，该值指示当前捕获模式
capture.get(10)   图像的亮度(仅适用于照相机)
capture.get(11)   图像的对比度(仅适用于照相机)
capture.get(12)   图像的饱和度(仅适用于照相机)
capture.get(13)   色调图像(仅适用于照相机)
capture.get(14)   图像增益(仅适用于照相机)（Gain在摄影中表示白平衡提升）
capture.get(15)   曝光(仅适用于照相机)
capture.get(16)   指示是否应将图像转换为RGB布尔标志
capture.get(17)   × 暂时不支持
capture.get(18)   立体摄像机的矫正标注（目前只有DC1394 v.2.x后端支持这个功能）
"""

frame_count = int(capture.get(cv2.CAP_PROP_FPS))