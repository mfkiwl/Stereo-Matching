function [image_points_left,image_points_right]=calculation_matched_points_roi(image_left,image_right,list_index)

%init this variable
image_points_ROI=[];

%counting the loop
count=0;

while size(image_points_ROI,1)<max(list_index)

    if count>0

        disp(' ')
        disp('==> WARNING: Checkboard not detected, please select again.')

    end

    %ROI from left and right image
    [ROI_left,boundary_left]=calculation_boundary_roi(image_left,'Left');
    [ROI_right,boundary_right]=calculation_boundary_roi(image_right,'Right');

    [ROI_left_new,ROI_right_new] = calculation_combination_roi(ROI_left,ROI_right);

    %relative coordinates in combination set
    image_points_ROI= detectCheckerboardPoints(ROI_left_new,ROI_right_new);

    disp(' ')
    disp(['=> ',num2str(size(image_points_ROI,1)),' points detected.'])

    count=count+1;

end

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