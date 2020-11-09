%  calculate y-shift of the original stereo images
function [y_shift] = calculation_vertical_difference(image_points_left,...
                                                     image_points_right)

% all 15x20=300 feature points
points_shift=image_points_left-image_points_right;
shift_x_y_mean_std =[mean(points_shift) std(abs(points_shift))];

% offset in x and y direction
x_shift=shift_x_y_mean_std(1);
y_shift=shift_x_y_mean_std(2);