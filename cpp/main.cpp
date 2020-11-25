// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-Veritical Difference Calculation

******************************************************************************/

// main.cpp: This file contains the "main" function.
// This is where program execution begins and ends

#include "init.h"

#include "Header\operation_video.h"

#include "Header\calculation_feature_matching.h"

int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "../Material";

	////left and right image name
	string name_image_a = "L13.jpg";
	string name_image_b = "R13.jpg";

	//string name_image_a = "L3.bmp";
	//string name_image_b = "R3.bmp";

	Mat image_a = imread(folder_path + "/" + name_image_a);
	Mat image_b = imread(folder_path + "/" + name_image_b);

	if (!image_a.data || !image_b.data){

		cout << "==> ERROR: Images not found" << endl;
		return -1;
	}
	//determine the order of duals
	vector<Mat> vector_image = DualCamerasOrder(image_a, image_b);

	Mat image_left = vector_image[0];
	Mat image_right = vector_image[1];

	//calculate vertical difference between left and right images
	double y_shift = CalculateVerticalDifference(image_left, image_right);

	//string fileName = "D:/工作/工业相机3D视频素材/20201113_143105.avi";

	//VideoStreaming(fileName);
}