function [image_points]=calculation_image_points_roi(image,...
                                                     list_index,...
                                                     str_title)

%init this variable
image_points_ROI=[];

%counting the loop
count=0;

%self-defined ROI
while size(image_points_ROI,1)<max(list_index)

    if count>0

        disp(' ')
        disp('==> WARNING: Checkboard in this image not detected, please select again.')

    end

    %ROI from the image
    [ROI,boundary]=calculation_boundary_roi(image,str_title);

    %relative coordinates in combination set
    image_points_ROI= detectCheckerboardPoints(ROI,ROI);

%     disp(' ')
%     disp(['=> ',num2str(size(image_points_ROI,1)),' points detected in the image.'])

    count=count+1;

end

%boundary parameter
row_min=boundary(1);
col_min=boundary(3);

%image points from ROI
image_points=image_points_ROI(list_index,:,1,1);

%compensate the offset
image_points(:,1)=image_points(:,1)+col_min;
image_points(:,2)=image_points(:,2)+row_min;

