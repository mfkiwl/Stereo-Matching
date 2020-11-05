function [image_points]=calculation_matched_points(image_left,image_right,list_index)

%detect checkerboards in images
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