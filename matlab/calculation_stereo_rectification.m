% calculate y-shift from AVT real images after rectification (for test image 01, 02, 03, 04)
%
% Cai, 2020-Oct-23

% Load the stereoParameters object
load('D:\Kamerawerk\Projects\徐工双目\Matlab\AVTstereoParams.mat');

%% calculate y-shift of the original stereo images
% test image 
I1 = imread('D:\Kamerawerk\Projects\徐工双目\Calib\shift\Left01.png');
I2 = imread('D:\Kamerawerk\Projects\徐工双目\Calib\shift\Right01.png');

% Detect checkerboards in images
imagePoints  = detectCheckerboardPoints(I1, I2);
% Visualize candidate matches
figure;
showMatchedFeatures(I1,I2,imagePoints([1 21 42],:,1,1),imagePoints([1 21 42],:,1,2),'montage');
% all 15x20=300 feature points
ptsShift = imagePoints(:,:,1,1)-imagePoints(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift)) std(abs(ptsShift))]


%% image rectification and calculate the y-shift
[J1, J2] = rectifyStereoImages(I1, I2, stereoParams);
 
% Detect checkerboards in images
imagePoints_rec = detectCheckerboardPoints(J1, J2);
% Visualize candidate matches
figure;
showMatchedFeatures(J1,J2,imagePoints_rec([1 21 42],:,1,1),imagePoints_rec([1 21 42],:,1,2),'montage');
% x- and y-shift 
ptsShift_rec = imagePoints_rec(:,:,1,1)-imagePoints_rec(:,:,1,2);
shift_x_y_mean_std = [mean(abs(ptsShift_rec)) std(abs(ptsShift_rec))]