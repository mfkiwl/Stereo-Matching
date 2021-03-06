// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: Source-Calculation on feature matching
******************************************************************************/

#include "..\Header\operation_vector.h"

#include "..\Header\calculation_feature_matching.h"
#include "..\Header\configuration_global_variable.h"

//------------------------------------------------------------------------------
/*
Calculate horizontal and vertical difference and determine which image is left or right

Args:
	Mat image_a: Mat object of image from camera 0
	Mat image_b: Mat object of image from camera 1
	display: (bool) whether to imshow

Returns:
	horizontal and vertical difference between left and right images
*/
pair<bool, vector<double>> CalculateDifference(Mat& image_a,
												Mat& image_b,
												const string& match_operator,
												bool display = false) {

	cout << endl << "-- Calculate Difference" << endl;
	cout << "==> operator: " << match_operator << endl;

	//feature matching detector
	Ptr<Feature2D>detector;

	if (match_operator == "ORB") {

		detector = ORB::create(NUM_FEATURE_POINTS);
	}
	if (match_operator == "SIFT") {

		detector = SIFT::create(NUM_FEATURE_POINTS);
	}
	if (match_operator == "SURF") {

		detector = SURF::create(HESSION_THRESHOLD);
	}
	//define vector of key points
	vector<KeyPoint> key_points_a;
	vector<KeyPoint> key_points_b;

	//define descriptor
	Mat descriptor_a;
	Mat descriptor_b;

	//detect and compute descriptor
	detector->detectAndCompute(image_a, Mat(), key_points_a, descriptor_a);
	detector->detectAndCompute(image_b, Mat(), key_points_b, descriptor_b);

	//match with Brute Force method or Flann Based
	BFMatcher matcher;
	//FlannBasedMatcher matcher;

	//vector of DMatch which stores information about some of the points in it 
	vector<DMatch> matches;

	//match left and right images
	matcher.match(descriptor_a, descriptor_b, matches, Mat());

	if (display) {

		//display the matching result of first step
		Mat image_matches;

		drawMatches(image_a,
					key_points_a,
					image_b,
					key_points_b,
					matches,
					image_matches);

		namedWindow("Feature Matching (original)", cv::WINDOW_NORMAL);
		resizeWindow("Feature Matching (original)", 1600, 450);
		imshow("Feature Matching (original)", image_matches);
		waitKey(666);
	}
	//slope of all key points
	vector<double> slope_key_points;

	//threshold of slope
	double slope_threshold = 0.6;

	//search interval
	int n_interval = 200;

	//select a suitable slop from this list
	vector<double> vector_slope_candidate;

	//traverse and collect the candidate slope
	for (double this_slope = -slope_threshold; this_slope < slope_threshold; this_slope += (slope_threshold / n_interval)) {

		vector_slope_candidate.push_back(this_slope);
		//cout << this_slope << endl;
	}
	//collect the final slope
	vector_slope_candidate.push_back(slope_threshold);
	//cout << vector_slope_candidate.size() << endl;

	//vector to collect distance sum for each slope 
	for (int k = 0; k < matches.size(); ++k) {

		//left key point
		double x_key_points_a = key_points_a[matches[k].queryIdx].pt.x;
		double y_key_points_a = key_points_a[matches[k].queryIdx].pt.y;

		//right key point
		double x_key_points_b = key_points_b[matches[k].trainIdx].pt.x;
		double y_key_points_b = key_points_b[matches[k].trainIdx].pt.y;

		//diff of x and y
		double diff_x = x_key_points_a - x_key_points_b;
		double diff_y = y_key_points_a - y_key_points_b;

		//slope
		slope_key_points.push_back(diff_y / (diff_x - image_a.cols));

		//cout << k << " " << matches[k].queryIdx << " " << matches[k].trainIdx << endl;
		//cout << slope_key_points[k] << endl;
	}
	//write slope into txt file
	ofstream out_file;
	out_file.open("../Outcome/slope.txt");

	for (int k = 0; k < slope_key_points.size(); k++) {

		out_file << slope_key_points[k] << endl;
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
	cout << "==> matched slope: " << matched_slope << endl;

	//list good matched result
	vector<DMatch> good_matches;

	//traverse all slopes and make classification to define good matches
	for (int k = 0; k < slope_key_points.size(); ++k) {

		//the key points whose slope is near matched slop may be considered
		if (abs(slope_key_points[k] - matched_slope) < slope_threshold / n_interval) {

			good_matches.push_back(matches[k]);
		}
	}
	cout << "==> amount of matches: " << matches.size() << endl;
	cout << "==> amount of good matches: " << good_matches.size() << endl;

	//vector to collect x and y shift
	vector<double> vector_x_diff;
	vector<double> vector_y_diff;

	//coordinates from matchesn
	for (int k = 0; k < good_matches.size(); ++k) {

		//left key point
		double x_key_points_a = key_points_a[good_matches[k].queryIdx].pt.x;
		double y_key_points_a = key_points_a[good_matches[k].queryIdx].pt.y;

		//right key point
		double x_key_points_b = key_points_b[good_matches[k].trainIdx].pt.x;
		double y_key_points_b = key_points_b[good_matches[k].trainIdx].pt.y;

		//diff of x and y
		double diff_x = x_key_points_a - x_key_points_b;
		double diff_y = y_key_points_a - y_key_points_b;

		//diff in x and y direction
		vector_x_diff.push_back(diff_x);
		vector_y_diff.push_back(diff_y);

		//cout << diff_x << endl;
		//cout << diff_y << endl;
	}
	double x_shift = VectorAverage(vector_x_diff);
	double y_shift = VectorAverage(vector_y_diff);

	cout << "==> x shift: " << x_shift << endl;
	cout << "==> y shift: " << y_shift << endl;

	//vector to store x y shift
	vector<double> vector_shift;

	vector_shift.push_back(x_shift);
	vector_shift.push_back(y_shift);

	//output result
	pair<bool, vector<double>> pair_output;

	//init
	pair_output.first = true;
	pair_output.second = vector_shift;

	if (good_matches.size() < THRESHOLD_NUM_GOOD_MATCHES) {
	
		pair_output.first = false;
	}
	if (display) {

		Mat image_good_matches;
		drawMatches(image_a,
					key_points_a,
					image_b,
					key_points_b,
					good_matches,
					image_good_matches);

		namedWindow("Feature Matching (good)", WINDOW_NORMAL);
		resizeWindow("Feature Matching (good)", 1600, 450);
		imshow("Feature Matching (good)", image_good_matches);
		waitKey(666);
	}
	return pair_output;
}
int CheckOutOrder(Mat& image_a, 
				Mat& image_b, 
				const string& match_operator) {

	pair<bool, vector<double>>pair_output = CalculateDifference(image_a, image_b, match_operator);

	//invalid output: insufficent matches
	if (!pair_output.first) {
	
		return -1;
	}
	//horizontal difference
	double x_shift = pair_output.second[0];

	if (x_shift > 0) {

		cout << "--> Conclusion: Image A is left, and image B is right." << endl;

		return 1;
	}
	else {

		cout << "--> Conclusion: Image B is left, and image A is right." << endl;

		return 0;
	}
}
vector<Mat> DualCamerasOrder(Mat& image_a,
							Mat& image_b,
							const string& match_operator) {
		
	cout << endl << "-- Dual Order" << endl;
	cout << "==> operator: " << match_operator << endl;

	bool flag = CheckOutOrder(image_a, image_b, match_operator);
	
	//vector to store 2 images
	vector<Mat> vector_image;

	if (flag) {

		vector_image.push_back(image_a);
		vector_image.push_back(image_b);
	}
	else {

		vector_image.push_back(image_b);
		vector_image.push_back(image_a);
	}

	return vector_image;
}
pair<bool, double> CalculateVerticalDifference(Mat& image_left,
												Mat& image_right,
												const string& match_operator) {
	
	cout << endl << "-- Calculate Vertical Difference" << endl;
	cout << "==> operator: " << match_operator << endl;

	//output result
	pair<bool, double> pair_output;

	//fet result from last step
	pair<bool, vector<double>> pair_input = CalculateDifference(image_left, image_right, match_operator, false);
	
	//horizontal difference
	double y_shift = pair_input.second[1];

	//select mutiple of 4
	double y_shift_final = round(y_shift / ACCURACY_Y_SHIFT) * ACCURACY_Y_SHIFT;

	//init result
	pair_output.first = pair_input.first;
	pair_output.second = y_shift;
	//pair_output.second = y_shift_final;

	if (y_shift > 0) {

		cout << "--> Result: Right image is "
			<< abs(y_shift_final)
			<< " (+"
			<< abs(y_shift)
			<< " in reality) "
			<< "pixels higher than left one"
			<< endl;
	}
	else {

		cout << "--> Result: Left image is "
			<< abs(y_shift_final)
			<< " (-"
			<< abs(y_shift)
			<< " in reality) "
			<< "pixels higher than right one"
			<< endl;
	}
	return pair_output;
}
	