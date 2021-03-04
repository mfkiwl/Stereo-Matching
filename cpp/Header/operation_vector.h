// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: Header-Calculation on Vector
******************************************************************************/

#include "..\init.h"

#ifndef _CALCULATION_VECTOR_H_
#define _CALCULATION_VECTOR_H_

//Calculate sum of vector
int VectorSum(vector<int>& which_vector);
float VectorSum(vector<float>& which_vector);
double VectorSum(vector<double>& which_vector);

//Calculate average of vector
double VectorAverage(vector<int>& which_vector);
double VectorAverage(vector<float>& which_vector);
double VectorAverage(vector<double>& which_vector);

//Calculate the index of maximum in a vector
int MaximumIndex(vector<double>& which_vector);

//Calculate the index of minimum in a vector
int MinimumIndex(vector<double>& which_vector);

#endif