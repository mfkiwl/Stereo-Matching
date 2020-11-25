# -*- coding: utf-8 -*-
"""
Created on Fri Nov 13 20:35:12 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：Module-Feature Matching

"""

import cv2

import numpy as np
from matplotlib import pyplot as plt

from configuration_font import legend_prop,\
                               text_prop,\
                               label_prop,\
                               title_prop,\
                               annotation_prop
                               
def bgr_rgb(img):
    
    (r, g, b) = cv2.split(img)
    
    return cv2.merge([b, g, r])

def DrawMatch(img1, img2, kp1, kp2, match):
    
    outimage = cv2.drawMatches(img1, kp1, img2, kp2, match, outImg=None)
    plt.imshow(bgr_rgb(outimage))

def DrawMatchedLines(key_points_left,key_points_right,image_left):
    
    for k in range(len(key_points_left)):
        
        plt.plot([key_points_left[k].pt[0],key_points_right[k].pt[0]+np.shape(image_left)[1]],
                 [key_points_left[k].pt[1],key_points_right[k].pt[1]],
                 '-',
                 linewidth=.13)
        
def MergeDualImages(image_left,image_right):
    
    if np.shape(image_left)!=np.shape(image_right):
        
        print('=> ERROR: Incorrect Dimension of Dual Images')
        
        return
    
    image_merged=np.zeros((np.shape(image_left)[0],
                           np.shape(image_left)[1]*2,
                           3))
    
    #left image
    image_merged[:,:np.shape(image_left)[1],:]=image_left[:,:,:]
    image_merged[:,np.shape(image_left)[1]:np.shape(image_left)[1]*2,:]=image_right[:,:,:]
          
    return image_merged.astype(image_left[0,0,0])

def DrawDualImages(image_left,image_right):
    
    plt.imshow(bgr_rgb(MergeDualImages(image_left,image_right)))
    
def KeyPointsFromMatches(key_points_left,
                         key_points_right,
                         matches):
    
    key_points_left_sorted=[]
    key_points_right_sorted=[]
    
    for this_match in matches:

        #obtain key points from matching result
        key_points_left_sorted.append(key_points_left[this_match.queryIdx])
        key_points_right_sorted.append(key_points_right[this_match.trainIdx])
 
        # print(matches.index(this_match),this_match.queryIdx,this_match.trainIdx)
        
    return key_points_left_sorted,key_points_right_sorted

def SlopeFromKeyPoints(key_points_left,key_points_right,image_left):
    
    slope_key_points=[]
    
    for k in range(len(key_points_left)):
        
        point_left=key_points_left[k]
        point_right=key_points_right[k]
        
        diff_x=point_left.pt[0]-point_right.pt[0]
        diff_y=point_left.pt[1]-point_right.pt[1]
    
        #calculate slope of key points
        this_slope=diff_y/(diff_x-np.shape(image_left)[1])

        slope_key_points.append(this_slope)
    
    return slope_key_points 

def DrawMatchedPoints(image_left,
                      key_points_left,
                      key_points_right):

    #draw good match on left image
    for this_left_point in key_points_left:
        
        plt.plot(this_left_point.pt[0],
                 this_left_point.pt[1],
                 'r.',
                 markersize=2.3)
        
    #draw good match on right image
    for this_right_point in key_points_right:
        
        plt.plot(this_right_point.pt[0]+np.shape(image_left)[1],
                 this_right_point.pt[1],
                 'b.',
                 markersize=2.3)
        
def CalculateGoodMatches(slope_key_points,matches):
    
    #max slope
    slope_threshold=0.6
    n_interval=200
    
    #list to collect distance sum for each slope 
    list_distance_for_each_slope=[]
    
    #select a suitable slop from this list
    list_slope_candidate=list(np.linspace(-slope_threshold,slope_threshold,n_interval))
    
    #find the best slope
    for this_slope_candidate in list_slope_candidate:
        
        this_distance=0
        
        for this_slope in slope_key_points:
            
            this_distance+=np.abs(this_slope-this_slope_candidate)
    
        list_distance_for_each_slope.append(this_distance)
        
    #minimum of the sum of the distance between candidate and real slope counts
    index_min=list_distance_for_each_slope.index(np.min(list_distance_for_each_slope))
   
    #suitable slope from matching result
    matched_slope=list_slope_candidate[index_min]
    
    print('==> matched slope:',matched_slope)
    
    plt.figure(figsize=(13,6))
    
    #list good matched result
    good_matches=[]
    
    #traverse all slopes and make classification
    for k in range(len(slope_key_points)):
        
        this_slope=slope_key_points[k]
        
        #the key points whose slope is near matched slop may be considered
        if abs(this_slope-matched_slope)<slope_threshold/n_interval:
    
            good_matches.append(matches[k])
            
            plt.plot(k,this_slope,'k.')
            
        else:
                
            plt.plot(k,this_slope,'g.')

    plt.hlines(y=matched_slope-3*slope_threshold/n_interval,
               xmin=0,
               xmax=len(slope_key_points),
               color='r',
               linestyle='--')
    
    plt.hlines(y=matched_slope+3*slope_threshold/n_interval,
               xmin=0,
               xmax=len(slope_key_points),
               color='r',
               linestyle='--')
    
    plt.vlines(x=0,
               ymin=matched_slope-3*slope_threshold/n_interval,
               ymax=matched_slope+3*slope_threshold/n_interval,
               color='r',
               linestyle='--')
    
    plt.vlines(x=len(slope_key_points),
               ymin=matched_slope-3*slope_threshold/n_interval,
               ymax=matched_slope+3*slope_threshold/n_interval,
               color='r',
               linestyle='--')
    
    plt.ylim([-slope_threshold,slope_threshold])
    
    plt.ylabel('Slope (-)',fontdict=label_prop)
    plt.xlabel('Key Points (-)',fontdict=label_prop)

    plt.title('Slope Distribution of all Key Points',fontdict=title_prop)
    
    #set ticks fonts
    plt.tick_params(labelsize=12)
    labels=plt.gca().get_xticklabels()+plt.gca().get_yticklabels()
    
    #label fonts
    [this_label.set_fontname('Times New Roman') for this_label in labels]
    
    print('==> amount of matches:',len(matches))
    print('==> amount of good matches:',len(good_matches))
    
    return good_matches
    
def CalculateVerticalDifference(key_points_left,key_points_right):
    
    #y coordinate of key points
    y_key_points_left=[this_key_point.pt[1] for this_key_point in key_points_left]
    y_key_points_right=[this_key_point.pt[1] for this_key_point in key_points_right]
    
    return np.average(np.array(y_key_points_left)-np.array(y_key_points_right))
    
def CalculateHorizontalDifference(key_points_left,key_points_right):
    
    #x coordinate of key points
    x_key_points_left=[this_key_point.pt[0] for this_key_point in key_points_left]
    x_key_points_right=[this_key_point.pt[0] for this_key_point in key_points_right]
    
    return np.average(np.array(x_key_points_left)-np.array(x_key_points_right))