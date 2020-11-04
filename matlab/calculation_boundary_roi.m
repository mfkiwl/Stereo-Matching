function [ROI,boundary] = calculation_boundary_roi(image)

%left image
figure;
imshow(image);
title('Left Image: Please define ROI whose boundary was from your click');

% define ROI boundary box from points selected
[x,y] = ginput(4);

% boundary coordinates
x_min=round(min(x));
x_max=round(max(x));
y_min=round(min(y));
y_max=round(max(y));

close;

%boundary parameter
boundary=[y_min,y_max,x_min,x_max];

%determine ROI
ROI=image(y_min:y_max,x_min:x_max);

end