% calculate y-shift from AVT real images after rectification (for test image 01, 02, 03, 04)
%
% Cai, 2020-Nov-4

clc;
clear;
close all;

% Load the stereoParameters object
% load('AVTstereoParams.mat');

% calculate y-shift of the original stereo images
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
list_index=[1 21 41];

% Detect checkerboards in images
image_points= detectCheckerboardPoints(image_left,image_right);

image_points_left=image_points(list_index,:,1,1);
image_points_right=image_points(list_index,:,1,2);

% Visualize candidate matches
figure;
showMatchedFeatures(image_left,...
                    image_right,...
                    image_points_left,...
                    image_points_right,...
                    'montage');
title('Original Image: Feature Points Matching')

%whether to excute ROI definition
[bool_AB,bool_BC,bool_CA]=calculation_decision_roi(image_points,list_index);

if not(bool_AB&bool_BC&bool_CA)||(size(imagePoints ,1)==0)
    
    close
    
    %ROI from left and right image
    [ROI_left,boundary_left]=calculation_boundary_roi(image_left,'Left');
    [ROI_right,boundary_right]=calculation_boundary_roi(image_right,'Right');

    [ROI_left_new,ROI_right_new] = calculation_combination_roi(ROI_left,ROI_right);

    %relative coordinates in combination set
    image_points_ROI= detectCheckerboardPoints(ROI_left_new,ROI_right_new);
    
    %boundary parameter
    row_min_left=boundary_left(1);
    col_min_left=boundary_left(3);
    row_min_right=boundary_right(1);
    col_min_right=boundary_right(3);
    
    %image points from ROI
    image_points_left=image_points_ROI(list_index,:,1,1);
    image_points_right=image_points_ROI(list_index,:,1,2);
    
    %compensate the offset
    %left image
    image_points_left(:,1)=image_points_left(:,1)+col_min_left;
    image_points_left(:,2)=image_points_left(:,2)+row_min_left;
    
    %right image
    image_points_right(:,1)=image_points_right(:,1)+col_min_right;
    image_points_right(:,2)=image_points_right(:,2)+row_min_right;
    
    figure;
    showMatchedFeatures(image_left,...
                        image_right,...
                        image_points_left,...
                        image_points_right,...
                        'montage');
    title('Original Image: Feature Points Matching')
    
    % all 15x20=300 feature points
    ptsShift = image_points_left-image_points_right;
    shift_x_y_mean_std = [mean(ptsShift) std(abs(ptsShift))];

    % offset in x and y direction
    x_shift=shift_x_y_mean_std(1);
    y_shift=4*round(shift_x_y_mean_std(2)/4);

    [rows,cols]=size(image_left);
    
    %define rectified image points
    image_points_left_rec=image_points_left;
    image_points_right_rec=image_points_right;
        
    % rectify in y direction
    if y_shift>0
        
        image_left_rec=image_left(1+y_shift:rows,:,:);
        image_right_rec=image_right(1:rows-y_shift,:,:);
        image_points_left_rec(:,2)=image_points_left_rec(:,2)-y_shift;
    end
    
    if y_shift<0
        
        image_left_rec=image_left(1-y_shift:rows,:,:);
        image_right_rec=image_right(1:y_shape+rows,:,:);
        image_points_right_rec(:,2)=image_points_right_rec(:,2)+y_shift;
    end
    
    figure;
    
    % Visualize candidate matches
    showMatchedFeatures(image_left_rec,...
                        image_right_rec,...
                        image_points_left_rec,...
                        image_points_right_rec,...
                        'montage');      
     title(['After Y-Shift Rectification:  Right image is ',num2str(abs(y_shift)),' pixels higher than left one']);
else

    % all 15x20=300 feature points
    ptsShift = image_points_left-image_points_right;
    shift_x_y_mean_std = [mean(ptsShift) std(abs(ptsShift))];

    % offset in x and y direction
    x_shift=shift_x_y_mean_std(1);
    y_shift=4*round(shift_x_y_mean_std(2)/4);

    [rows,cols]=size(image_left);

    % rectify in y direction
    if y_shift>0    
        image_left_rec=image_left(1+y_shift:rows,:,:);
        image_right_rec=image_right(1:rows-y_shift,:,:);
    end
    
    if y_shift<0      
        image_left_rec=image_left(1-y_shift:rows,:,:);
        image_right_rec=image_right(1:y_shape+rows,:,:);     
    end
    
    title(['After Y-Shift Rectification:  Right image is ',num2str(abs(y_shift)),' pixels higher than left one']);

    figure;

    % Detect checkerboards in images
    image_points_rec = detectCheckerboardPoints(image_left_rec,image_right_rec);

    % Visualize candidate matches

    showMatchedFeatures(image_left_rec,...
                        image_right_rec,...
                        image_points_rec(list_index,:,1,1),...
                        image_points_rec(list_index,:,1,2),...
                        'montage');
end

if y_shift<0
    
    title(['After Y-Shift Rectification: Left image is ',num2str(abs(y_shift)),' pixels higher than right one']);

end
if y_shift>0
    
    title(['After Y-Shift Rectification:  Right image is ',num2str(abs(y_shift)),' pixels higher than left one']);
end


