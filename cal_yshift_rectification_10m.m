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
% I1 = imread([folder_path,'L2.png']);
% I2 = imread([folder_path,'R2.png']);

% right image 2:right image
I1 = imread([folder_path,'L4.bmp']);
I2 = imread([folder_path,'R4.bmp']);

disp(' ')
disp('...')
disp('......')
disp('正在提取特征点，稍等嗷~');

% Detect checkerboards in images
imagePoints  = detectCheckerboardPoints(I1, I2);

% Visualize candidate matches
figure;
showMatchedFeatures(I1,I2,imagePoints([1 21 41],:,1,1),imagePoints([1 21 41],:,1,2),'montage');
title('原图')

% select 3 dual points to check out
point_A_left=imagePoints([1],:,1,1);
point_B_left=imagePoints([21],:,1,1);
point_C_left=imagePoints([41],:,1,1);

point_A_right=imagePoints([1],:,1,2);
point_B_right=imagePoints([21],:,1,2);
point_C_right=imagePoints([41],:,1,2);

% calculate the difference
diff_A=point_A_left-point_A_right;
diff_B=point_B_left-point_B_right;
diff_C=point_C_left-point_C_right;

% calculate the inclination
slope_A=diff_A(1)/diff_A(2);
slope_B=diff_B(1)/diff_B(2);
slope_C=diff_C(1)/diff_C(2);

% diff of slope
diff_slope=0.03;

% determine whether to calculate
bool_AB=((abs(abs(slope_A)-abs(slope_B)))<diff_slope)&(slope_A/slope_B>0);
bool_BC=((abs(abs(slope_B)-abs(slope_C)))<diff_slope)&(slope_B/slope_C>0);
bool_CA=((abs(abs(slope_C)-abs(slope_A)))<diff_slope)&(slope_C/slope_A>0);

if not(bool_AB&bool_BC&bool_CA)|(size(imagePoints ,1)==0)
    
    disp(' ')
    disp('只好手动拾取ROI咯~')
    disp('请点四个点圈定ROI，四个点乱序哦~')
    disp('注：ROI尽可能大于标定板~')
    
    figure;
    imshow(I1);
    title('原图：请提取ROI')
    
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

    imagePoints  = detectCheckerboardPoints(I1, I2);
    
    figure;
    showMatchedFeatures(I1,I2,imagePoints([1 21 41],:,1,1),imagePoints([1 21 41],:,1,2),'montage');
    title('原图（ROI）')
    
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

y_shape=size(I1,1);
x_shape=size(I2,2);

% rectify in y direction
if y_shift>0
   
    K1=I1(1+y_shift:y_shape,:,:);
    K2=I2(1:y_shape-y_shift,:,:);
    
    disp(' ')
    disp(['Y Shift为正：右图比左图高',num2str(abs(y_shift)),'个pixel']);

end
if y_shift<0
    
    K1=I1(1-y_shift:y_shape,:,:);
    K2=I2(1:y_shape+y_shift,:,:);
    
    disp(' ')
    disp(['Y Shift为负：左图比右图高',num2str(abs(y_shift)),'个pixel']);

end

% Detect checkerboards in images
imagePoints_rec = detectCheckerboardPoints(K1, K2);
% Visualize candidate matches
figure;
showMatchedFeatures(K1,K2,imagePoints_rec([1 21 41],:,1,1),imagePoints_rec([1 21 41],:,1,2),'montage');
title('Y方向校正后')

% x- and y-shift 
ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))];

