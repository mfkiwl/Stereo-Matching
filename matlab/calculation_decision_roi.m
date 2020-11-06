function [bool_AB,bool_BC,bool_CA]=calculation_decision_roi(image_points_left_right,list_index)

index_A=list_index(1);
index_B=list_index(2);
index_C=list_index(3);

%select 3 dual points to check out
point_A_left=image_points_left_right([index_A],:,1,1);
point_B_left=image_points_left_right([index_B],:,1,1);
point_C_left=image_points_left_right([index_C],:,1,1);

point_A_right=image_points_left_right([index_A],:,1,2);
point_B_right=image_points_left_right([index_B],:,1,2);
point_C_right=image_points_left_right([index_C],:,1,2);

%calculate the difference
diff_A=point_A_left-point_A_right;
diff_B=point_B_left-point_B_right;
diff_C=point_C_left-point_C_right;

%calculate the inclination
slope_A=diff_A(1)/diff_A(2);
slope_B=diff_B(1)/diff_B(2);
slope_C=diff_C(1)/diff_C(2);

%diff of slope
diff_slope=0.03;

%determine whether to calculate
bool_AB=((abs(abs(slope_A)-abs(slope_B)))<diff_slope)&(slope_A/slope_B>0);
bool_BC=((abs(abs(slope_B)-abs(slope_C)))<diff_slope)&(slope_B/slope_C>0);
bool_CA=((abs(abs(slope_C)-abs(slope_A)))<diff_slope)&(slope_C/slope_A>0);

end