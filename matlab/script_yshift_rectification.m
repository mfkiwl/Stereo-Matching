% calculate y-shift from AVT real images after rectification (for test image 01, 02, 03, 04)
%
% Wei, 2020-Nov-4

clc;
clear;
close all;

% calculate y-shift of the original stereo images

% test image folder
folder_path='Material\';

% 1:left image 2:right image
% image_left=imread([folder_path,'L1.png']);
% image_right=imread([folder_path,'R1.png']);

image_left = imread([folder_path,'L3.bmp']);
image_right = imread([folder_path,'R3.bmp']);

%feature points index
list_index=[1 21 41];

%match feature points
image_points_matrix=calculation_matched_points(image_left,image_right,list_index);

%whether to excute ROI definition
[bool_AB,bool_BC,bool_CA]=calculation_decision_roi(image_points_matrix,list_index);

%first decision deteced points did exist and suitable slope
if size(image_points_matrix,1)==0||not(bool_AB&&bool_BC&&bool_CA)
    
    %calculate matched points after rectification from self-defined ROI
    calculation_rectified_matched_points_roi(image_left,image_right,list_index);

else 
    
    %calculate matched points after rectification from orignal image
    calculation_rectified_matched_points(image_left,image_right,list_index); 

end
        
                  

