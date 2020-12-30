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

Mat CenterROI(Mat& image_gray) {

	Point_<int> center_ROI;
	center_ROI.x = int(image_gray.cols / 2);
	center_ROI.y = int(image_gray.rows / 2);

	//ROI factor
	double zoom_factor = 0.9;

	//define ROI size
	int width_ROI = int(image_gray.cols * zoom_factor);
	int height_ROI = int(image_gray.rows * zoom_factor);

	//define ROI parameter
	Rect rect(center_ROI.x - int(width_ROI / 2),
		center_ROI.y - int(height_ROI / 2),
		width_ROI,
		height_ROI);

	return image_gray(rect);
}
Mat DisparityMapBM(Mat& image_left, Mat& image_right) {

	//gray level to calculate disparity
	Mat image_gray_left;
	Mat image_gray_right;

	cvtColor(image_left, image_gray_left, COLOR_BGR2GRAY);
	cvtColor(image_right, image_gray_right, COLOR_BGR2GRAY);
	
	image_gray_left = image_gray_left;
	image_gray_right = image_gray_right;

	//Mat of disparity
	Mat disparity, disparity_8U;

	int block_size = 9;
	int num_disparities = 16 * 8;
	int uniqueness_ratio = 15;

	Ptr<StereoBM> BM = StereoBM::create(16, 9);

	//BM->setROI1(ROI);
	//BM->setROI2(ROI);
	BM->setPreFilterCap(31);
	BM->setBlockSize(block_size);
	BM->setMinDisparity(-16);
	BM->setNumDisparities(num_disparities);
	BM->setTextureThreshold(10);
	BM->setUniquenessRatio(uniqueness_ratio);
	BM->setSpeckleWindowSize(100);
	BM->setSpeckleRange(32);
	BM->setDisp12MaxDiff(1);
	BM->compute(CenterROI(image_gray_left), CenterROI(image_gray_right), disparity);

	disparity.convertTo(disparity_8U, CV_8UC1, 255 / (num_disparities));
    
	/*namedWindow("Disparity (BM)", 1);
	imshow("Disparity (BM)", disparity_8U);
	waitKey(0);*/

	return disparity;
}
Mat DisparityMapSGBM(Mat& image_left, Mat& image_right) {

	//gray level to calculate disparity
	Mat image_gray_left;
	Mat image_gray_right;

	cvtColor(image_left, image_gray_left, COLOR_BGR2GRAY);
	cvtColor(image_right, image_gray_right, COLOR_BGR2GRAY);

	image_gray_left = image_gray_left;
	image_gray_right = image_gray_right;

	//Mat of disparity
	Mat disparity, disparity_8U;
	int block_size = 9;
	int num_disparities = 16 * 8;
	int uniqueness_ratio = 15;
	int P1 = image_left.channels() * block_size * block_size * 8;
	int P2 = image_left.channels() * block_size * block_size * 32;

	Ptr<StereoSGBM> SGBM = StereoSGBM::create(16, 9);

	//BM->setROI1(ROI);
	//BM->setROI2(ROI);
	SGBM->setPreFilterCap(31);
	SGBM->setBlockSize(block_size);
	SGBM->setMinDisparity(-16);
	SGBM->setNumDisparities(num_disparities);
	SGBM->setUniquenessRatio(uniqueness_ratio);
	SGBM->setSpeckleWindowSize(100);
	SGBM->setSpeckleRange(32);
	SGBM->setDisp12MaxDiff(1);
	SGBM->setP1(P1);
	SGBM->setP2(P2); 
	SGBM->setMode(StereoSGBM::MODE_SGBM_3WAY); //MODE_HH MODE_SGBM MODE_SGBM_3WAY
	SGBM->compute(CenterROI(image_gray_left), CenterROI(image_gray_right), disparity);

	disparity.convertTo(disparity_8U, CV_8UC1, 255 / (num_disparities));

	namedWindow("Disparity (SGBM)", 1);
	imshow("Disparity (SGBM)", disparity_8U);
	waitKey(0);

	return disparity;
}
int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "../Material";

	//left and right image name
	string name_image_a = "L13.jpg";
	string name_image_b = "R13.jpg";

	//string name_image_a = "L3.bmp";
	//string name_image_b = "R3.bmp";

    //string name_image_a = "L14.png";
    //string name_image_b = "R14.png";

	Mat image_a = imread(folder_path + "/" + name_image_a);
	Mat image_b = imread(folder_path + "/" + name_image_b);

	if (!image_a.data || !image_b.data){

		cout << "==> ERROR: Images not found" << endl;
		return -1;
	}
	//determine the order of duals
	vector<Mat> vector_image = DualCamerasOrder(image_a, image_b, MATCH_OPERATOR);

	Mat image_left = vector_image[0];
	Mat image_right = vector_image[1];

	for (int i = 0; i < 10; i++) {

		double t1 = clock();

		//DisparityMapSGBM(image_a, image_b);
		//calculate vertical difference between left and right images
		double y_shift = CalculateVerticalDifference(image_left, image_right, MATCH_OPERATOR).second;
		
		double t2 = clock();

		cout << "==> time consumed: " << (t2 - t1) / CLOCKS_PER_SEC * 1000 << endl;
	}
	
	//calculate vertical difference between left and right images
	//double y_shift = CalculateVerticalDifference(image_left, image_right, "SURF");

	//string fileName = "D:/工作/工业相机3D视频素材/20201113_143105.avi";

	//VideoStreaming(fileName);
}
