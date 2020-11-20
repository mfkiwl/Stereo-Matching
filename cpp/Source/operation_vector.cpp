// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: Source-Calculation on Vector
******************************************************************************/

#include "..\Header\operation_vector.h"

//Calculate sum of vector
//overloaded function 1: vector of int type
int VectorSum(vector<int>& which_vector) {

	//final result
	int sum = 0;

	for (int i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//overloaded function 2: vector of float type
float VectorSum(vector<float>& which_vector) {

	//final result
	float sum = 0;

	for (int i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//overloaded function 3: vector of double type
double VectorSum(vector<double>& which_vector) {

	//final result
	double sum = 0;

	for (int i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//Calculate average of vector
//overloaded function 1: vector of int type
double VectorAverage(vector<int>& which_vector) {

	int sum = VectorSum(which_vector);

	return double(sum) / which_vector.size();
}
//overloaded function 2: vector of float type
double VectorAverage(vector<float>& which_vector) {

	float sum = VectorSum(which_vector);

	return double(sum) / which_vector.size();
}
//overloaded function 2: vector of double type
double VectorAverage(vector<double>& which_vector) {

	double sum = VectorSum(which_vector);

	return double(sum) / which_vector.size();
}
//Calculate the index of maximum in a vector
int MaximumIndex(vector<double>& which_vector) {

	int index_maximum = 0;

	//traverse all element
	for (int k = 0; k < which_vector.size(); k++) {

		if (which_vector[k] > which_vector[index_maximum]) {

			index_maximum = k;
		}
	}
	return index_maximum;
}
//Calculate the index of minimum in a vector
int MinimumIndex(vector<double>& which_vector) {

	int index_minimum = 0;

	//traverse all element
	for (int k = 0; k < which_vector.size(); k++) {

		if (which_vector[k] < which_vector[index_minimum]) {

			index_minimum = k;
		}
	}
	return index_minimum;
}