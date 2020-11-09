function [bool_AB_x,...
          bool_BC_x,...
          bool_CA_x]=calculation_decision_roi(image_points_left,...
                                              image_points_right,...
                                              rows,...
                                              cols)
                                        
%select 3 dual points to check out
point_A_left=image_points_left(1,:);
point_B_left=image_points_left(2,:);
point_C_left=image_points_left(3,:);

point_A_right=image_points_right(1,:);
point_B_right=image_points_right(2,:);
point_C_right=image_points_right(3,:);

%calculate the difference
diff_A=point_A_left-point_A_right;
diff_B=point_B_left-point_B_right;
diff_C=point_C_left-point_C_right;

%calculate the inclination
slope_A_x=diff_A(2)/cols;
slope_B_x=diff_B(2)/cols;
slope_C_x=diff_C(2)/cols;
% 
% slope_A_y=diff_A(1)/rows;
% slope_B_y=diff_B(1)/rows;
% slope_C_y=diff_C(1)/rows;

%diff of slope
diff_slope=0.001;

%determine whether to calculate from x y slope
bool_AB_x=((abs(abs(slope_A_x)-abs(slope_B_x)))<diff_slope)&&(slope_A_x/slope_B_x>0);
bool_BC_x=((abs(abs(slope_B_x)-abs(slope_C_x)))<diff_slope)&&(slope_B_x/slope_C_x>0);
bool_CA_x=((abs(abs(slope_C_x)-abs(slope_A_x)))<diff_slope)&&(slope_C_x/slope_A_x>0);

% bool_AB_y=((abs(abs(slope_A_y)-abs(slope_B_y)))<diff_slope)&&(slope_A_y/slope_B_y>0);
% bool_BC_y=((abs(abs(slope_B_y)-abs(slope_C_y)))<diff_slope)&&(slope_B_y/slope_C_y>0);
% bool_CA_y=((abs(abs(slope_C_y)-abs(slope_A_y)))<diff_slope)&&(slope_C_y/slope_A_y>0);

%final determination
% bool_AB=bool_AB_x||bool_AB_y;
% bool_BC=bool_BC_x||bool_BC_y;
% bool_CA=bool_CA_x||bool_CA_y;

end