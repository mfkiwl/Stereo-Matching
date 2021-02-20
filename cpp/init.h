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
#include <fstream>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;

using cv::Ptr;
using cv::Mat;
using cv::ORB;
using cv::Rect;
using cv::Size;
using cv::SIFT;
using cv::Point;
using cv::Point_;
using cv::Scalar;
using cv::DMatch;
using cv::BFMatcher;
using cv::KeyPoint;
using cv::StereoBM;
using cv::StereoSGBM;
using cv::FlannBasedMatcher;
using cv::VideoCapture;
using cv::Feature2D;
using cv::xfeatures2d::SURF;

using cv::putText;
using cv::imread;
using cv::imshow;
using cv::waitKey;
using cv::namedWindow;
using cv::resizeWindow;
using cv::drawMatches;

using cv::CAP_PROP_FPS;
using cv::WINDOW_NORMAL;
using cv::COLOR_BGR2GRAY;
using cv::FONT_HERSHEY_PLAIN;
using cv::CAP_PROP_FRAME_COUNT;
using cv::CAP_PROP_FRAME_WIDTH;
using cv::CAP_PROP_FRAME_HEIGHT;