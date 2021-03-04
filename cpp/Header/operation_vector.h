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
INT VectorSum(vector<INT>& which_vector);
FLOAT VectorSum(vector<FLOAT>& which_vector);
DOUBLE VectorSum(vector<DOUBLE>& which_vector);

//Calculate average of vector
DOUBLE VectorAverage(vector<INT>& which_vector);
DOUBLE VectorAverage(vector<FLOAT>& which_vector);
DOUBLE VectorAverage(vector<DOUBLE>& which_vector);

//Calculate the index of maximum in a vector
INT MaximumIndex(vector<DOUBLE>& which_vector);

//Calculate the index of minimum in a vector
INT MinimumIndex(vector<DOUBLE>& which_vector);

#endif