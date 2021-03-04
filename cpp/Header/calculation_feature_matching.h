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

pair<BOOL, vector<DOUBLE>> CalculateDifference(Mat& image_a, Mat& image_b, CONST string& match_operator, BOOL display);

INT CheckOutOrder(Mat& image_a, Mat& image_b, CONST string& match_operator);

vector<Mat> DualCamerasOrder(Mat& image_a, Mat& image_b, CONST string& match_operator);

//Calculate vertical difference and determine which image is left or right
pair<BOOL, DOUBLE> CalculateVerticalDifference(Mat& image_left, Mat& image_right, CONST string& match_operator);

#endif