// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: Initialization Script
******************************************************************************/

#pragma once

#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

using cv::Ptr;
using cv::Mat;
using cv::ORB;
using cv::SIFT;
using cv::DMatch;
using cv::BFMatcher;
using cv::KeyPoint;
using cv::FlannBasedMatcher;

using cv::imshow;
using cv::waitKey;
using cv::namedWindow;
using cv::resizeWindow;
using cv::drawMatches;

using cv::WINDOW_NORMAL;

#define ACCURACY_Y_SHIFT 5