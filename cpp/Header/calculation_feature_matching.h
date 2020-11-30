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

vector<double> CalculateDifference(Mat& image_a, Mat& image_b, bool display);

bool CheckOutOrder(Mat& image_a, Mat& image_b);

vector<Mat> DualCamerasOrder(Mat& image_a, Mat& image_b);

//Calculate vertical difference and determine which image is left or right
double CalculateVerticalDifference(Mat& image_left, Mat& image_right);

#endif