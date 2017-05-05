/*
 * common.cpp
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#include "common.h"

unsigned int getPossibleBatchesCount(vector<int> * elements, int k)
{
	unsigned int minPartsCount = UINT_MAX;
	int * countingTable = new int[k + 1];
	for(int i = 0; i < elements->size(); i++)
	{
		countingTable[elements->at(i)]++;
	}
	for(int i = 1; i < k+1; i++)
	{
		if(countingTable[i] < minPartsCount)
			minPartsCount = countingTable[i];
	}
	return minPartsCount;
}

inline const bool are2PartsInCorrectOrder(vector<int> * elements,
		int k, int firstPosition, int secPosition)
{
	if (firstPosition + 1 != secPosition)
		return false;
	if (((elements->at(firstPosition)) % k + 1) == elements->at(secPosition))
		return true;
	return false;
}

bool isSetFound(vector<int> * vec, int k, int index)
{
	for (int i = 0; i < k; i++)
	{
		if (vec->at(index) != i + 1)
			return false;
		index++;
	}
	return true;
}

bool isSetFoundInLastM(vector<int> * vec, int k, int startIndex)
{
	for (unsigned int i = startIndex; i <= vec->size() - k; i++)
	{
		if (isSetFound(vec, k, i))
			return true;
	}
	return false;
}

const bool isSortedFromXtoN(vector<int> * elements, int k, int startIndex)
{
	if (startIndex >= elements->size() - 1 || startIndex < 0)
		return false;
	for (; startIndex != elements->size() - 1; startIndex++)
	{
		if (!are2PartsInCorrectOrder(elements, k, startIndex, startIndex + 1))
			return false;
	}
	return true;
}

const bool isSortedFromXtoN(vector<int> * elements, int k, int startIndex,
		int NN)
{
	if (startIndex >= NN - 1 || startIndex < 0)
		return false;
	for (; startIndex != NN - 1; startIndex++)
	{
		if (!are2PartsInCorrectOrder(elements, k, startIndex, startIndex + 1))
			return false;
	}
	return true;
}

void printVector(vector<int> * vec)
{
	vector<int>::iterator it = vec->begin();
	for (; it != vec->end(); it++)
	{
		cout << *it << ", ";
	}
	cout << endl;
}
