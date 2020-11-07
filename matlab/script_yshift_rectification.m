%-*- coding: utf-8 -*-
%{
@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: script-calculate y-shift from AVT real images after rectification
%}

clc;
clear;
close all;

% calculate y-shift of the original stereo images

% test image folder
folder_path='Material\';

% 1:left image 2:right image
% image_left=imread([folder_path,'L5.png']);
% image_right=imread([folder_path,'R5.png']);
% 
image_left = imread([folder_path,'L3.bmp']);
image_right = imread([folder_path,'R3.bmp']);

%feature points index
list_index=[1 21 41];

%match feature points
[image_points_left,...
 image_points_right]=calculation_matched_points(image_left,...
                                                image_right,...
                                                list_index);

%first decision deteced points did exist
if size(image_points_left,1)==0||...
   size(image_points_right,1)==0

    %calculate matched points after rectification from self-defined ROI
    calculation_rectified_matched_points_roi(image_left,...
                                             image_right,...
                                             list_index);

else 
    
    %whether to excute ROI definition
    [bool_AB,...
     bool_BC,...
     bool_CA]=calculation_decision_roi(image_points_left,...
                                       image_points_right);

    if bool_AB&&...
       bool_BC&&...
       bool_CA
        
        %calculate matched points after rectification from orignal image
        calculation_rectified_matched_points(image_left,...
                                             image_right,...
                                             list_index);
                                         
    else
        
        %calculate matched points after rectification from self-defined ROI
        calculation_rectified_matched_points_roi(image_left,...
                                                 image_right,...
                                                 list_index);  
                                             
    end
 
end

