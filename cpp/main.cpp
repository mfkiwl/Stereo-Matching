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
//Calculate sum of vector
//overloaded function 1: vector of int type
int VectorSum(vector<int>& which_vector) {

	//final result
	int sum = 0;

	for (int i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//overloaded function 2: vector of float type
float VectorSum(vector<float>& which_vector) {

	//final result
	float sum = 0;

	for (int i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//overloaded function 3: vector of double type
double VectorSum(vector<double>& which_vector) {

	//final result
	double sum = 0;

	for (int i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}

//Calculate average of vector
//overloaded function 1: vector of int type
double VectorAverage(vector<int>& which_vector) {

	int sum = VectorSum(which_vector);

	return double(sum) / which_vector.size();
}
//overloaded function 2: vector of float type
double VectorAverage(vector<float>& which_vector) {

	float sum = VectorSum(which_vector);

	return double(sum) / which_vector.size();
}
//overloaded function 2: vector of double type
double VectorAverage(vector<double>& which_vector) {

	double sum = VectorSum(which_vector);

	return double(sum) / which_vector.size();
}
int MaximumIndex(vector<double>& which_vector) {

	int index_maximum = 0;

	//traverse all element
	for (int k = 0; k < which_vector.size(); k++) {

		if (which_vector[k] > which_vector[index_maximum]) {

			index_maximum = k;
		}
	}
	return index_maximum;
}
int MinimumIndex(vector<double>& which_vector) {

	int index_minimum = 0;

	//traverse all element
	for (int k = 0; k < which_vector.size(); k++) {

		if (which_vector[k] < which_vector[index_minimum]) {

			index_minimum = k;
		}
	}
	return index_minimum;
}
int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "D:\\GitHub\\KAMERAWERK\\Binocular-Stereo-Matching\\matlab\\Material";

	//left and right image name
	string name_image_left = "L11.jpg";
	string name_image_right = "R11.jpg";

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
	//Ptr<SIFT> sift = SIFT::create();
	
	//ORB
	//cv::Ptr<cv::ORB> orb = cv::ORB::create();
	//SURF
	//Ptr<SURF> surf = SURF::create(400);

	//vector<KeyPoint> key_points_SIFT, key_points_SURF;

	//sift->detect(image_left, keypoints_SIFT);
	//surf->detect(image_left, keypoints_SIFT);

	//cout << "SIFT: " << key_points_SIFT.size() << endl;
	//cout << "SURF" << key_points_SURF.size() << endl;

	//启始时间
	//double t1 = getTickCount();

	//特征点提取
	Ptr<ORB> detector = ORB::create(n_feature_points);
	vector<KeyPoint> keypoints_left;
	vector<KeyPoint> keypoints_right;

	//定义描述子
	Mat descriptor_left, descriptor_right;

	//检测并计算成描述子
	detector->detectAndCompute(image_left, Mat(), keypoints_left, descriptor_left);
	detector->detectAndCompute(image_right, Mat(), keypoints_right, descriptor_right);

	//结束时间
	//double t2 = getTickCount();
	//double t = (t2 - t1) * 1000 / getTickFrequency();

	//特征匹配
	FlannBasedMatcher fbmatcher(new flann::LshIndexParams(20, 10, 2));
	vector<DMatch> matches;

	//将找到的描述子进行匹配并存入matches中
	fbmatcher.match(descriptor_left, descriptor_right, matches);

	//cout << matches.size() << endl;
	//cout << keypoints_left.size() << endl;

	vector<KeyPoint> key_points_left;
	vector<KeyPoint> key_points_right;

	vector<double> slope_key_points;

	//threshold of slope
	double slope_threshold = 0.2;

	//search interval
	int n_interval = 50;

	//select a suitable slop from this list
	vector<double> vector_slope_candidate;

	//traverse and collect the candidate slope
	for (double this_slope = -slope_threshold; this_slope <slope_threshold; this_slope += (slope_threshold / n_interval)) {
	
		vector_slope_candidate.push_back(this_slope);
		//cout << this_slope << endl;
	}

	//collect the final slope
	vector_slope_candidate.push_back(slope_threshold);
	//cout << vector_slope_candidate.size() << endl;

	//vector to collect distance sum for each slope 
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
		slope_key_points.push_back(diff_y / diff_x);

		//cout<< keypoints_left[matches[k].queryIdx].pt.x<<endl;
		//cout << keypoints_right[matches[k].trainIdx].pt.y << endl;
		//cout << slope_key_points[k] << endl;
	}
	//list to collect distance sum for each slope
	vector<double>vector_distance_for_each_slope;

	//find the best slope
	for (int k = 0; k < vector_slope_candidate.size(); ++k) {
	
		double this_distance = 0.0;

		for (int kk = 0; kk < slope_key_points.size(); ++kk) {
		
			this_distance += abs(slope_key_points[kk] - vector_slope_candidate[k]);
		}
		vector_distance_for_each_slope.push_back(this_distance);
	}
	//minimum of the sum of the distance between candidate and real slope counts
	int  index_min = MinimumIndex(vector_distance_for_each_slope);

	//suitable slope from matching result
	double matched_slope = vector_slope_candidate[index_min];

	//list good matched result
	vector<DMatch> good_matches;

	//traverse all slopes and make classification
	for (int k = 0; k < slope_key_points.size(); ++k) {
	
		//the key points whose slope is near matched slop may be considered
		if (abs(slope_key_points[k] - matched_slope) < slope_threshold / n_interval) {
			
			good_matches.push_back(matches[k]);
		}
	}
	cout << matches.size() << endl;
	//vector to collect x and y shift
	vector<double> vector_x_shift;
	vector<double> vector_y_shift;

	//coordinates from matchesn
	for (int k = 0; k < good_matches.size(); ++k) {

		//left key point
		double x_keypoints_left = keypoints_left[matches[k].queryIdx].pt.x;
		double y_keypoints_left = keypoints_left[matches[k].queryIdx].pt.y;

		//right key point
		double x_keypoints_right = keypoints_left[matches[k].trainIdx].pt.x;
		double y_keypoints_right = keypoints_left[matches[k].trainIdx].pt.y;

		//diff of x and y
		double diff_x = x_keypoints_right - x_keypoints_left + image_left.cols;
		double diff_y = y_keypoints_right - y_keypoints_left;

		//diff in x and y direction
		vector_x_shift.push_back(diff_x);
		vector_y_shift.push_back(diff_y);
	}

	double y_shift = VectorAverage(vector_y_shift);
	cout << y_shift << endl;
	return 0;
}