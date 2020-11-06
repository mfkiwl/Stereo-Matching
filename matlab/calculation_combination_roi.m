%--------------------------------------------------------------------------
%{
Definition of ROI

Args:
    ROI_A
    ROI_B
    
Returns:
    ROI_A_new
    ROI_B_new
%}
function [ROI_A_new,ROI_B_new] = calculation_combination_roi(ROI_A,ROI_B)

%number of row column of ROI
[row_A,col_A]=size(ROI_A);
[row_B,col_B]=size(ROI_B);

%size of new ROI
row=max([row_A,row_B]);
col=max([col_A,col_B]);

%define a new matrix as ROI
ROI_A_new=zeros(row,col);
ROI_B_new=zeros(row,col);

%give original value to new matrix
ROI_A_new(1:row_A,1:col_A)=ROI_A;
ROI_B_new(1:row_B,1:col_B)=ROI_B;

end



