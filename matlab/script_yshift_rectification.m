%-*- coding: utf-8 -*-
%{
Created on Mon Nov  9 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: script-calculate y-shift from binocular images after rectification
%}

clc; 
clear;
close all;

% test image folder
folder_path='../Material/';

% 1:left image 2:right image
% image_left=imread([folder_path,'L11.png']);
% image_right=imread([folder_path,'R11.png']);

image_left = imread([folder_path,'L3.bmp']);
image_right = imread([folder_path,'R3.bmp']);

% image_left=imread([folder_path,'L13.jpg']);
% image_right=imread([folder_path,'R13.jpg']);

%size of dual images
[rows,cols]=calculation_image_size(image_left,...
                                   image_right);
                                          
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
                                       image_points_right,...
                                       rows,...
                                       cols);

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
