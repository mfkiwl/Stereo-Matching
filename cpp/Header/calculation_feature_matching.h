// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: Header-Calculation on feature matching
******************************************************************************/

#include "..\init.h"

#ifndef _CALCULATION_FEATURE_MATCHING_H_
#define _CALCULATION_FEATURE_MATCHING_H_

pair<bool, vector<double>> CalculateDifference(Mat& image_a, Mat& image_b, const string& match_operator, bool display);

bool CheckOutOrder(Mat& image_a, Mat& image_b, const string& match_operator);

vector<cv::Mat> DualCamerasOrder(Mat& image_a, Mat& image_b, const string& match_operator);

//Calculate vertical difference and determine which image is left or right
pair<bool, double> CalculateVerticalDifference(Mat& image_left, Mat& image_right, const string& match_operator);

#endif