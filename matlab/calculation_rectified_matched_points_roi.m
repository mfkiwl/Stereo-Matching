function [image_points_left_rec,image_points_rec_right]=calculation_rectified_match_points_roi(image_left,image_right,list_index)

title('Original Image: Feature Points Matching (Incorrect)')
% close

%get matched points from roi definition
[image_points_left,image_points_right]=calculation_match_points_roi(image_left,image_right,list_index);
    
y_shift=calculation_vertical_difference(image_points_left,image_points_right);

[rows,~]=size(image_left);
    
%define rectified image points
image_points_rec_left=image_points_left;
image_points_rec_right=image_points_right;

% rectify in y direction
if y_shift>0

    image_left_rec=image_left(1+y_shift:rows,:,:);
    image_right_rec=image_right(1:rows-y_shift,:,:);
    image_points_left_rec(:,2)=image_points_rec_left(:,2)-y_shift;

end

if y_shift<0

    image_left_rec=image_left(1-y_shift:rows,:,:);
    image_right_rec=image_right(1:y_shape+rows,:,:);
    image_points_rec_right(:,2)=image_points_rec_right(:,2)+y_shift;

end

figure;

% Visualize candidate matches
showMatchedFeatures(image_left_rec,...
                    image_right_rec,...
                    image_points_rec_left,...
                    image_points_rec_right,...
                    'montage');
                
%print vital info
if y_shift<0
    
    title(['Y-Shift Rectification: Left image is ',num2str(abs(y_shift)),' pixels higher than right one']);

end

if y_shift>0
    
    title(['Y-Shift Rectification:  Right image is ',num2str(abs(y_shift)),' pixels higher than left one']);

end