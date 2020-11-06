function [image_points_matrix]=calculation_matched_points(image_left,image_right,list_index)

disp(' ')
disp('...')
disp('......')
disp('-- Extracting Feature Points, please wait...');

%detect checkerboards in images
image_points_matrix=detectCheckerboardPoints(image_left,image_right);

%init output variable
image_points_left=image_points_matrix(list_index,:,1,1);
image_points_right=image_points_matrix(list_index,:,1,2);

% Visualize candidate matches
figure;
showMatchedFeatures(image_left,...
                    image_right,...
                    image_points_left,...
                    image_points_right,...
                    'montage');
title('Feature Points Matching (Original Image)')