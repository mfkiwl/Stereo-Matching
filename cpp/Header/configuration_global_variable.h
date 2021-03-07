// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Feb 11 14:51:37 2021

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title£ºHeader-configuration of global variable
******************************************************************************/

#pragma once

#include "../init.h"

//-------------------------------const VARIABLE-------------------------------
extern const int ACCURACY_Y_SHIFT;
extern const int HESSION_THRESHOLD;
extern const int NUM_FEATURE_POINTS;
extern const int THRESHOLD_NUM_GOOD_MATCHES;

extern const string MATCH_OPERATOR;

extern const int WIDTH;
extern const int HEIGHT;

//mapping matrix
extern Mat mapping_x_left;
extern Mat mapping_y_left;
extern Mat mapping_x_right;
extern Mat mapping_y_right;