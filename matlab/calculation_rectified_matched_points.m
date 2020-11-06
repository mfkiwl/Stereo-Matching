function [image_points_rec_left,image_points_rec_right]=calculation_rectified_matched_points(image_left,image_right,list_index)

close

%get matched points from roi definition
image_points_matrix=calculation_matched_points(image_left,image_right,list_index);

%image points drom left and right image
image_points_left=image_points_matrix(list_index,:,1,1);
image_points_right=image_points_matrix(list_index,:,1,2);

%calculate difference in y direction
y_shift=calculation_vertical_difference(image_points_left,image_points_right);

[rows,~]=size(image_left);

% rectify in y direction
if y_shift>0    

    image_left_rec=image_left(1+y_shift:rows,:,:);
    image_right_rec=image_right(1:rows-y_shift,:,:);

end

if y_shift<0     

    image_left_rec=image_left(1-y_shift:rows,:,:);
    image_right_rec=image_right(1:y_shape+rows,:,:);   

end

figure;

% Detect checkerboards in images
image_points_rec = detectCheckerboardPoints(image_left_rec,image_right_rec);

%rectified matched points from left and right image
image_points_rec_left=image_points_rec(list_index,:,1,1);
image_points_rec_right=image_points_rec(list_index,:,1,2);

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