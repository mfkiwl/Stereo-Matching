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


Mat DisparityMapBM(Mat& image_left, Mat& image_right) {

	//gray level to calculate disparity
	Mat image_gray_left;
	Mat image_gray_right;

	cvtColor(image_left, image_gray_left, COLOR_BGR2GRAY);
	cvtColor(image_right, image_gray_right, COLOR_BGR2GRAY);

	Mat disparity, disparity_8U;

	int num_disparities = 128;
	int uniqueness_ratio = 16;

	Ptr<StereoBM> bm = StereoBM::create(16, 9);
	/*Rect roi1, roi2;
	bm->setROI1(roi1);
	bm->setROI2(roi2);*/
	bm->setPreFilterCap(31);
	bm->setBlockSize(9);
	bm->setMinDisparity(0);
	bm->setNumDisparities(num_disparities);
	bm->setTextureThreshold(10);
	bm->setUniquenessRatio(uniqueness_ratio);
	bm->setSpeckleWindowSize(100);
	bm->setSpeckleRange(32);
	bm->setDisp12MaxDiff(1);
	bm->compute(image_gray_left, image_gray_right, disparity);

	disparity.convertTo(disparity_8U, CV_8UC1, 255 / (num_disparities));
    
	namedWindow("Disparity (BM)", 0);
	imshow("Disparity (BM)", disparity_8U);
	waitKey(0);

	return disparity;
}

int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "../Material";

	////left and right image name
	//string name_image_a = "L13.jpg";
	//string name_image_b = "R13.jpg";

	//string name_image_a = "L3.bmp";
	//string name_image_b = "R3.bmp";

    string name_image_a = "L14.png";
    string name_image_b = "R14.png";

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

	DisparityMapBM(image_left, image_right);

	//calculate vertical difference between left and right images
	//double y_shift = CalculateVerticalDifference(image_left, image_right);

	//string fileName = "D:/工作/工业相机3D视频素材/20201113_143105.avi";

	//VideoStreaming(fileName);
}
