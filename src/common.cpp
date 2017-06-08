/*
 * common.cpp
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#include "common.h"

const int findPartOnTheRight(vector<int> * elements, unsigned int startIndex,
		int partNumber)
{
	for (; startIndex < elements->size(); startIndex++)
	{
		if (elements->at(startIndex) == partNumber)
			return startIndex;
	}
	return -1;
}

const int findPartOnTheLeft(vector<int> * elements, unsigned int startIdxIncl,
		int partNumber, unsigned int leftBorderIdxIncl)
{
	for (; startIdxIncl >= leftBorderIdxIncl; startIdxIncl--)
	{
		if (elements->at(startIdxIncl) == partNumber)
			return startIdxIncl;
	}
	return -1;
}

inline const bool are2PartsInCorrectOrder(vector<int> * elements, int k,
		int firstPosition, int secPosition)
{
	if (firstPosition + 1 != secPosition)
		return false;
	if (((elements->at(firstPosition)) % k + 1) == elements->at(secPosition))
		return true;
	return false;
}

unsigned int getPossibleBatchesCount(vector<int> * elements, int k)
{
	unsigned int * countingTable;
	unsigned int minPartsCount = UINT_MAX;
	countingTable = new unsigned int[k + 1]();
	for (unsigned int i = 0; i < elements->size(); i++)
	{
		countingTable[elements->at(i)]++;
	}
	for (int i = 1; i < k + 1; i++)
	{
		if (countingTable[i] < minPartsCount)
			minPartsCount = countingTable[i];
	}
	delete [] countingTable;
	return minPartsCount;
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

bool isSetFoundInLastMmatchedToLeft(vector<int> * vec, int k, int startIndex,
		int placesOnTheLeftToSort)
{
	int leftBatchElemCounter = 1;
	for (unsigned int i = startIndex; i <= vec->size() - k;
			i++)
	{
		if(leftBatchElemCounter <= placesOnTheLeftToSort)
		{
			if(vec->at(i) != k - placesOnTheLeftToSort + leftBatchElemCounter)
				return false;
			leftBatchElemCounter++;
		}
		else
		{
			if (isSetFound(vec, k, i))
				return true;
		}
	}
	return false;
}

bool isSequenceMatchedToTheLeft(vector<int> * vec, int k, int startIndex,
		int placesOnTheLeftToSort)
{
	int leftBatchElemCounter = 1;
	for (unsigned int i = startIndex; i < startIndex + placesOnTheLeftToSort;
			i++)
	{
		if(vec->at(i) != k - placesOnTheLeftToSort + leftBatchElemCounter)
			return false;
		leftBatchElemCounter++;
	}
	return true;
}

const bool isSortedFromXtoN(vector<int> * elements, int k, unsigned int startIndex)
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

void printHistogram(vector<int> * vec, int k)
{
	unsigned int minPartsCount = UINT_MAX;
	unsigned int * countingTable = new unsigned int[k + 1]();
	for (unsigned int i = 0; i < vec->size(); i++)
	{
		countingTable[vec->at(i)]++;
	}
	for (int i = 1; i < k + 1; i++)
	{
		if (countingTable[i] < minPartsCount)
			minPartsCount = countingTable[i];
		cout<<"Part "<<i<<": "<<countingTable[i]<<endl;
	}
	delete [] countingTable;
	cout<<"Batches: "<<minPartsCount<<endl;
}

int getBatchesCount(vector<int> * vec, int k)
{
	unsigned int minPartsCount = UINT_MAX;
	unsigned int * countingTable = new unsigned int[k + 1]();
	for (unsigned int i = 0; i < vec->size(); i++)
	{
		countingTable[vec->at(i)]++;
	}
	for (int i = 1; i < k + 1; i++)
	{
		if (countingTable[i] < minPartsCount)
			minPartsCount = countingTable[i];
	}
	delete [] countingTable;
	return minPartsCount;
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
