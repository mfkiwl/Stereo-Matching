function [rows,cols] = calculation_image_size(image_left,...
                                              image_right)
                                          
[rows_left,cols_left]=size(image_left);
[rows_right,cols_right]=size(image_right);

%number of rows
rows=rows_left;

if rows>=rows_right
    
    rows=rows_right;
    
end

%number of columns
cols=cols_left;

if cols>=cols_right
    
    cols=cols_right;
    
end