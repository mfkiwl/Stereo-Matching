%--------------------------------------------------------------------------
%{
Definition of ROI

Args:
    image: input image
    str_title: string to be displayed in the screen
    
Returns:
    ROI: ROI matrix
    boundary: parameter of boundary [row_min,row_max,col_min,col_max]
%}
function [ROI,boundary] = calculation_boundary_roi(image,str_title)

%left image
figure;
imshow(image);
title([str_title,' Image: Please define ROI whose boundary was from your click']);

% define ROI boundary box from points selected
[col,row] = ginput(4);

% boundary coordinates
col_min=round(min(col));
col_max=round(max(col));
row_min=round(min(row));
row_max=round(max(row));

close;

%boundary parameter
boundary=[row_min,row_max,col_min,col_max];

%determine ROI
ROI=image(row_min:row_max,col_min:col_max);

end