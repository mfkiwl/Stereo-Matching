function [image_points_left,image_points_right]=calculation_matched_points_roi(image_left,image_right,list_index)

%left image
image_points_left=calculation_image_points_roi(image_left,list_index,'Left');

%right image
image_points_right=calculation_image_points_roi(image_right ,list_index,'Right');

figure;
showMatchedFeatures(image_left,...
                    image_right,...
                    image_points_left,...
                    image_points_right,...
                    'montage');
title('Original Image: Feature Points Matching')