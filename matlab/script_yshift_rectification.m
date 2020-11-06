% calculate y-shift from AVT real images after rectification (for test image 01, 02, 03, 04)
%
% Cai, 2020-Nov-4

clc;
clear;
close all;

% calculate y-shift of the original stereo images
% test image 
folder_path='Material\';

% 1:left image
% I1 = imread([folder_path,'L2.png']);
% I2 = imread([folder_path,'R2.png']);

% right image 2:right image
image_left = imread([folder_path,'L3.bmp']);
image_right = imread([folder_path,'R3.bmp']);

%feature points index
list_index=[1 21 41];

%match feature points
image_points_matrix=calculation_matched_points(image_left,image_right,list_index);

%first decision deteced points did exist
if size(image_points_matrix,1)~=0
    1
    %whether to excute ROI definition
    [bool_AB,bool_BC,bool_CA]=calculation_decision_roi(image_points_matrix,list_index);
    
    %calculate matched points after rectification
    if bool_AB&&bool_BC&&bool_CA
        2
        calculation_rectified_matched_points(image_left,image_right,list_index); 

    end

else
    3
    calculation_rectified_matched_points_roi(image_left,image_right,list_index);           

end