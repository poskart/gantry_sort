/*
 * common.cpp
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#include "common.h"

int K = 4;
int N = 14;


inline const bool are2PartsInCorrectOrder(vector<int> * elements, int firstPosition, int secPosition)
{
	if(firstPosition + 1 != secPosition)
		return false;
	if(((elements->at(firstPosition))%K + 1) == elements->at(secPosition))
		return true;
	return false;
}

bool isSetFound(vector<int> * vec, int index)
{
	for(int i = 0; i < K; i++)
	{
		if(vec->at(index) != i + 1)
			return false;
		index++;
	}
	return true;
}

bool isSetFoundInLastM(vector<int> * vec, int startIndex)
{
	for(int i = startIndex; i <= N - K; i++)
	{
		if(isSetFound(vec, i))
			return true;
	}
	return false;
}

const bool isSortedFromXtoN(vector<int> * elements, int startIndex)
{
	if(startIndex >= N-1 || startIndex < 0)
		return false;
	for(; startIndex != N - 1; startIndex++)
	{
		if(!are2PartsInCorrectOrder(elements, startIndex, startIndex + 1))
			return false;
	}
	return true;
}

const bool isSortedFromXtoN(vector<int> * elements, int startIndex, int NN)
{
	if(startIndex >= NN-1 || startIndex < 0)
		return false;
	for(; startIndex != NN - 1; startIndex++)
	{
		if(!are2PartsInCorrectOrder(elements, startIndex, startIndex + 1))
			return false;
	}
	return true;
}

void printVector(vector<int> * vec)
{
	vector<int>::iterator it = vec->begin();
	for(; it != vec->end(); it++)
	{
		cout<<*it<<", ";
	}
	cout<<endl;
}
