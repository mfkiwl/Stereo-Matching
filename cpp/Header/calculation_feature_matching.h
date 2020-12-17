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

vector<double> CalculateDifference(cv::Mat& image_a, cv::Mat& image_b, bool display);

bool CheckOutOrder(cv::Mat& image_a, cv::Mat& image_b);

vector<cv::Mat> DualCamerasOrder(cv::Mat& image_a, cv::Mat& image_b);

//Calculate vertical difference and determine which image is left or right
double CalculateVerticalDifference(cv::Mat& image_left, cv::Mat& image_right);

#endif