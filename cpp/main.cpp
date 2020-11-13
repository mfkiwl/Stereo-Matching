// -*- coding: utf-8 -*-
/******************************************************************************
@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: script-Self Calibration by Feature Matching
******************************************************************************/

// main.cpp: This file contains the "main" function.
// This is where program execution begins and ends

#include <string>
#include <vector>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int CalculateVerticalDifference() {

	return 0;
}

int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "D:\\GitHub\\KAMERAWERK\\Binocular-Stereo-Matching\\matlab\\Material";

	//left and right image name
	string name_image_left = "L8.bmp";
	string name_image_right = "R8.bmp";

	/*cout << folder_path + "\\" + name_image_left << endl;
	cout << folder_path + "\\" + name_image_right << endl;*/

	Mat image_left = imread(folder_path + "\\" + name_image_left);
	Mat image_right = imread(folder_path + "\\" + name_image_right);

	if (!image_left.data || !image_right.data)
	{
		cout << "==> ERROR: Images not found" << endl;
		return -1;
	}

	//提取特征点方法
	int n_feature_points = 1000;

	//SIFT
	Ptr<SIFT> sift = SIFT::create();
	
	//ORB
	//cv::Ptr<cv::ORB> orb = cv::ORB::create();
	//SURF
	//Ptr<SURF> surf = SURF::create(400);

	vector<KeyPoint> keypoints_SIFT, keypoints_SURF;

	sift->detect(image_left, keypoints_SIFT);
	//surf->detect(image_left, keypoints_SIFT);

	cout << "SIFT: " << keypoints_SIFT.size() << endl;
	//cout << "SURF" << keypoints_SURF.size() << endl;

	//amount of key points
	int Hession = 600;

	//启始时间
	double t1 = getTickCount();

	//特征点提取
	Ptr<ORB> detector = ORB::create(Hession);
	vector<KeyPoint> keypoints_left;
	vector<KeyPoint> keypoints_right;

	//定义描述子
	Mat descriptor_left, descriptor_right;

	//检测并计算成描述子
	detector->detectAndCompute(image_left, Mat(), keypoints_left, descriptor_left);
	detector->detectAndCompute(image_right, Mat(), keypoints_right, descriptor_right);

	//结束时间
	double t2 = getTickCount();
	double t = (t2 - t1) * 1000 / getTickFrequency();

	//特征匹配
	FlannBasedMatcher fbmatcher(new flann::LshIndexParams(20, 10, 2));
	vector<DMatch> matches;

	//将找到的描述子进行匹配并存入matches中
	fbmatcher.match(descriptor_left, descriptor_right, matches);

	cout << matches.size() << endl;
	cout << keypoints_left.size() << endl;

	vector<KeyPoint> key_points_left;
	vector<KeyPoint> key_points_right;

	vector<double> slope_key_points;

	for (int k = 0; k < matches.size(); ++k) {

		//left key point
		double x_keypoints_left = keypoints_left[matches[k].queryIdx].pt.x;
		double y_keypoints_left = keypoints_left[matches[k].queryIdx].pt.y;

		//right key point
		double x_keypoints_right = keypoints_left[matches[k].trainIdx].pt.x;
		double y_keypoints_right = keypoints_left[matches[k].trainIdx].pt.y;

		//diff of x and y
		double diff_x = x_keypoints_right - x_keypoints_left + image_left.cols;
		double diff_y = y_keypoints_right - y_keypoints_left;

		//slope
		//cout<< keypoints_left[matches[k].queryIdx].pt.x<<endl;
		//cout << keypoints_right[matches[k].trainIdx].pt.y << endl;
	}
	return 0;
}