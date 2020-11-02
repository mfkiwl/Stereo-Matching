% calculate y-shift from AVT real images after rectification (for test image 01, 02, 03, 04)
%
% Cai, 2020-Oct-23

clc;
clear;
close all;

% Load the stereoParameters object
load('AVTstereoParams.mat');

%% calculate y-shift of the original stereo images
% test image 
folder_path='C:\Users\ASUS\Desktop\徐工双目素材\';
% I1 = imread([folder_path,'Left02.png']);
% I2 = imread([folder_path,'Right02.png']);
I1 = imread([folder_path,'Left03.bmp']);
I2 = imread([folder_path,'Right03.bmp']);

% 3
x_ROI=[150,500];
y_ROI=[700,1200];

% % 4
% x_ROI=[250,450];
% y_ROI=[500,900];

% crop ROI
I1=I1(x_ROI(1):x_ROI(2),y_ROI(1):y_ROI(2));
I2=I2(x_ROI(1):x_ROI(2),y_ROI(1):y_ROI(2));

% Detect checkerboards in images
imagePoints  = detectCheckerboardPoints(I1, I2);
% Visualize candidate matches
figure;
showMatchedFeatures(I1,I2,imagePoints([1 11 21 31 41],:,1,1),imagePoints([1 11 21 31 41],:,1,2),'montage');
% all 15x20=300 feature points
ptsShift = imagePoints(:,:,1,1)-imagePoints(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift)) std(abs(ptsShift))]


%% image rectification and calculate the y-shift
[J1, J2] = rectifyStereoImages(I1, I2, stereoParams);
 
% Detect checkerboards in images  
imagePoints_rec = detectCheckerboardPoints(J1, J2);
% Visualize candidate matches
figure;
showMatchedFeatures(J1,J2,imagePoints_rec([1 11 21 31 41],:,1,1),imagePoints_rec([1 11 21 31 41],:,1,2),'montage');
% x- and y-shift 
ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))]

%% rectify in y direction
% offset in x and y direction
x_shift=shift_x_y_mean_std(1);
y_shift=shift_x_y_mean_std(2);

x_shape=size(J1,1);
y_shape=size(J2,2);

% rectify in y direction
K1=J1(:,y_shift:y_shape,:);
K2=J1(:,1:y_shape-y_shift+1,:);

% Detect checkerboards in images
imagePoints_rec = detectCheckerboardPoints(K1, K2);
% Visualize candidate matches
figure;
showMatchedFeatures(K1,K2,imagePoints_rec([1 11 21 31 41],:,1,1),imagePoints_rec([1 11 21 31 41],:,1,2),'montage');
% x- and y-shift 
ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))]