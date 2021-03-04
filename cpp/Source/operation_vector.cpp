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
//overloaded function 1: vector of INT type
INT VectorSum(vector<INT>& which_vector) {

	//final result
	INT sum = 0;

	for (INT i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//overloaded function 2: vector of FLOAT type
FLOAT VectorSum(vector<FLOAT>& which_vector) {

	//final result
	FLOAT sum = 0;

	for (INT i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//overloaded function 3: vector of DOUBLE type
DOUBLE VectorSum(vector<DOUBLE>& which_vector) {

	//final result
	DOUBLE sum = 0;

	for (INT i = 0; i < which_vector.size(); i++) {

		sum += which_vector[i];
	}
	return sum;
}
//Calculate average of vector
//overloaded function 1: vector of INT type
DOUBLE VectorAverage(vector<INT>& which_vector) {

	INT sum = VectorSum(which_vector);

	return DOUBLE(sum) / which_vector.size();
}
//overloaded function 2: vector of FLOAT type
DOUBLE VectorAverage(vector<FLOAT>& which_vector) {

	FLOAT sum = VectorSum(which_vector);

	return DOUBLE(sum) / which_vector.size();
}
//overloaded function 2: vector of DOUBLE type
DOUBLE VectorAverage(vector<DOUBLE>& which_vector) {

	DOUBLE sum = VectorSum(which_vector);

	return DOUBLE(sum) / which_vector.size();
}
//Calculate the index of maximum in a vector
INT MaximumIndex(vector<DOUBLE>& which_vector) {

	INT index_maximum = 0;

	//traverse all element
	for (INT k = 0; k < which_vector.size(); k++) {

		if (which_vector[k] > which_vector[index_maximum]) {

			index_maximum = k;
		}
	}
	return index_maximum;
}
//Calculate the index of minimum in a vector
INT MinimumIndex(vector<DOUBLE>& which_vector) {

	INT index_minimum = 0;

	//traverse all element
	for (INT k = 0; k < which_vector.size(); k++) {

		if (which_vector[k] < which_vector[index_minimum]) {

			index_minimum = k;
		}
	}
	return index_minimum;
}