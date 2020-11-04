% calculate y-shift from AVT real images after rectification (for test image 01, 02, 03, 04)
%
% Cai, 2020-Nov-4

clc;
clear;
close all;

% Load the stereoParameters object
% load('AVTstereoParams.mat');

%% calculate y-shift of the original stereo images
% test image 
folder_path='Material\';

% 1:left image
% I1 = imread([folder_path,'L2.png']);
% I2 = imread([folder_path,'R2.png']);

% right image 2:right image
image_left = imread([folder_path,'L3.bmp']);
image_right = imread([folder_path,'R3.bmp']);

disp(' ')
disp('...')
disp('......')
disp('-- Extracting Feature Points, please wait...');

%feature points index
index_A=1;
index_B=21;
index_C=41;

% Detect checkerboards in images
imagePoints  = detectCheckerboardPoints(image_left,image_right);

% Visualize candidate matches
figure;
showMatchedFeatures(image_left,...
                    image_right,...
                    imagePoints([index_A,index_B,index_C],:,1,1),...
                    imagePoints([index_A,index_B,index_C],:,1,2),...
                    'montage');
title('Original Image')

%whether to excute ROI definition
[bool_AB,bool_BC,bool_CA]=calculation_decision_roi(imagePoints,index_A,index_B,index_C);

if not(bool_AB&bool_BC&bool_CA)|(size(imagePoints ,1)==0)
    
    %ROI from left image
    ROI_left = calculation_boundary_roi(image_left);
    ROI_right = calculation_boundary_roi(image_right);

    [ROI_A_new,ROI_B_new] = calculation_combination_roi(ROI_A,ROI_B)
    
    imagePoints  = detectCheckerboardPoints(ROI_left, ROI_right);
    
    
    imagePoints([1 21 41],:,1,1)
    
    figure;
    showMatchedFeatures(image_left,...
                        image_right,...
                        imagePoints([index_A,index_B,index_C],:,1,1),...
                        imagePoints([index_A,index_B,index_C],:,1,2),...
                        'montage');
    
    title('Left Image (ROI)')
    
end

% all 15x20=300 feature points
ptsShift = imagePoints(:,:,1,1)-imagePoints(:,:,1,2);
shift_x_y_mean_std = [mean(ptsShift) std(abs(ptsShift))];

%% image rectification and calculate the y-shift
% [J1, J2] = rectifyStereoImages(I1, I2, stereoParams);
%  
% % Detect checkerboards in images  
% imagePoints_rec = detectCheckerboardPoints(J1, J2);
% % Visualize candidate matches
% figure;
% showMatchedFeatures(J1,J2,imagePoints_rec([1 11 21 31 41],:,1,1),imagePoints_rec([1 11 21 31 41],:,1,2),'montage');
% % x- and y-shift 
% ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
% shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))]

%% rectify in y direction
% offset in x and y direction
x_shift=shift_x_y_mean_std(1);
y_shift=4*round(shift_x_y_mean_std(2)/4);

[y_shape,x_shape]=size(I1);

% rectify in y direction
if y_shift>0
   
    K1=I1(1+y_shift:y_shape,:,:);
    K2=I2(1:y_shape-y_shift,:,:);
    
end
if y_shift<0
    
    K1=I1(1-y_shift:y_shape,:,:);
    K2=I2(1:y_shape+y_shift,:,:);

    title(['After Y-Shift Rectification:  Right image is ',num2str(abs(y_shift)),' pixels higher than left one']);
end
figure;

% Detect checkerboards in images
imagePoints_rec = detectCheckerboardPoints(K1, K2);
% Visualize candidate matches

showMatchedFeatures(K1,K2,imagePoints_rec([1 21 41],:,1,1),imagePoints_rec([1 21 41],:,1,2),'montage');

if y_shift<0
    
    title(['After Y-Shift Rectification: Left image is ',num2str(abs(y_shift)),' pixels higher than right one']);

end
if y_shift>0
    
    title(['After Y-Shift Rectification:  Right image is ',num2str(abs(y_shift)),' pixels higher than left one']);
end

% x- and y-shift 
ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))];

