// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Feb 11 14:51:37 2021

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title£ºSource-configuration of global variable
******************************************************************************/

#include "..\Header\configuration_global_variable.h"

//-------------------------------const VARIABLE-------------------------------
const int ACCURACY_Y_SHIFT = 2;
const int HESSION_THRESHOLD = 5000;
const int NUM_FEATURE_POINTS = 1000;
const int THRESHOLD_NUM_GOOD_MATCHES = 20;

const string MATCH_OPERATOR = "SURF";

const int WIDTH = 1920;
const int HEIGHT = 1080;

//mapping matrix
Mat mapping_x_left(HEIGHT, WIDTH, CV_32FC1);
Mat mapping_y_left(HEIGHT, WIDTH, CV_32FC1);
Mat mapping_x_right(HEIGHT, WIDTH, CV_32FC1);
Mat mapping_y_right(HEIGHT, WIDTH, CV_32FC1);
