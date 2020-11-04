// -*- coding: utf-8 -*-
/******************************************************************************
@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: script-Self Calibration
******************************************************************************/

// main.cpp: This file contains the "main" function.
// This is where program execution begins and ends

#include "highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/legacy/legacy.hpp"

using namespace cv;

int main(int argc, char* argv[])
{
	Mat image01 = imread(argv[1]);
	Mat image02 = imread(argv[2]);
	Mat image1, image2;
	GaussianBlur(image01, image1, Size(3, 3), 0.5);
	GaussianBlur(image02, image2, Size(3, 3), 0.5);

	//提取特征点
	SiftFeatureDetector siftDetector(30);  //限定提起前15个特征点
	vector<KeyPoint> keyPoint1, keyPoint2;
	siftDetector.detect(image1, keyPoint1);
	siftDetector.detect(image2, keyPoint2);

	//绘制特征点
	drawKeypoints(image1, keyPoint1, image1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(image2, keyPoint2, image2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	namedWindow("KeyPoints of image1", 0);
	namedWindow("KeyPoints of image2", 0);

	imshow("KeyPoints of image1", image1);
	imshow("KeyPoints of image2", image2);

	//特征点描述，为下边的特征点匹配做准备
	SiftDescriptorExtractor siftDescriptor;
	Mat imageDesc1, imageDesc2;
	siftDescriptor.compute(image1, keyPoint1, imageDesc1);
	siftDescriptor.compute(image2, keyPoint2, imageDesc2);

	//特征点匹配并显示匹配结果
	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
	Mat imageOutput;
	drawMatches(image01, keyPoint1, image02, keyPoint2, matchePoints, imageOutput);
	namedWindow("Mathch Points", 0);
	imshow("Mathch Points", imageOutput);
	waitKey();
	return 0;
}