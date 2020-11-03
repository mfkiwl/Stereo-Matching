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

% 1:left image
% I1 = imread([folder_path,'L1.png']);
% I2 = imread([folder_path,'R1.png']);

% right image 2:right image
I1 = imread([folder_path,'L3.bmp']);
I2 = imread([folder_path,'R3.bmp']);

figure;
imshow(I1);

% define ROI boundary box from points selected
[x,y] = ginput(4);

% boundary coordinates
x_min=round(min(x));
x_max=round(max(x));
y_min=round(min(y));
y_max=round(max(y));

close;

% crop ROI
I1=I1(y_min:y_max,x_min:x_max);
I2=I2(y_min:y_max,x_min:x_max);

% Detect checkerboards in images
imagePoints  = detectCheckerboardPoints(I1, I2);
% Visualize candidate matches
figure;
showMatchedFeatures(I1,I2,imagePoints([1 21 41],:,1,1),imagePoints([1 21 41],:,1,2),'montage');


% all 15x20=300 feature points
ptsShift = imagePoints(:,:,1,1)-imagePoints(:,:,1,2);
shift_x_y_mean_std = [mean(ptsShift) std(abs(ptsShift))]


% %% image rectification and calculate the y-shift
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

y_shape=size(I1,1);
x_shape=size(I2,2);

% rectify in y direction
if y_shift>0
   
    K1=I1(1+y_shift:y_shape,:,:);
    K2=I2(1:y_shape-y_shift,:,:);
    "y_shift为正：右图比左图高"
end
if y_shift<0
    
    K1=I1(1-y_shift:y_shape,:,:);
    K2=I2(1:y_shape+y_shift,:,:);
    "y_shift为负：右图比左图低"
end
% Detect checkerboards in images
imagePoints_rec = detectCheckerboardPoints(K1, K2);
% Visualize candidate matches
figure;
showMatchedFeatures(K1,K2,imagePoints_rec([1 21 41],:,1,1),imagePoints_rec([1 21 41],:,1,2),'montage');
% x- and y-shift 
ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))];


y_shift

