function [image_points]=calculation_image_points(image,list_index)

disp(' ')
disp('...')
disp('......')
disp('-- Extracting Feature Points, please wait...');

%match feature points
image_points_matrix=detectCheckerboardPoints(image,image);

%image points
image_points=image_points_matrix(list_index,:,1,1);


