/*
 * Sorter2.cpp
 *
 *  Created on: 5 maj 2017
 *      Author: piotr
 */

#include "Sorter2.h"

Sorter2::Sorter2(vector<int> * vec, int k, int n)
{
	this->elements = new vector<int>(*vec);
	this->elements->reserve(vec->size() + k);
	this->n = n;
	this->k = k;
}

Sorter2::~Sorter2()
{
	delete elements;
}

const int Sorter2::areSortedToTheLeft(vector<int> * elements,
		int startIndexIncluded)
{
	if (startIndexIncluded >= elements->size())
	{
		cout << "Error in Sorter2::areSortedToTheLeft()" << endl;
		return false;
	}
	int currentPart = elements->at(startIndexIncluded);
	int i = startIndexIncluded;
	while (currentPart != 1)
	{
		i--;
		if (i < 0 || elements->at(i) + 1 != currentPart)
			return 0;
		currentPart = elements->at(i);
	}
	return startIndexIncluded - i + 1;
}

/**
 * Find index of every K from right side which contains partIndex
 * and is between left and right borders.
 */
const int Sorter2::findValidIdxToMovePartAtPos(vector<int> * elements,
		int partIndex, int leftBorder, int rightBorder, int k)
{
	if (partIndex + k - 1 < rightBorder)
	{
		if (partIndex > leftBorder)
			return partIndex;
		else
			return -1;
	}
	else
	{
		while (partIndex + k - 1 >= rightBorder)
			partIndex--;
		if (partIndex > leftBorder)
			return partIndex;
		else
			return -1;
	}
}

int Sorter2::mvBatchMaxToTheLeft(vector<int> * el, int k, int batchCount,
		int firstBatchIdx, int batchIdx)
{
	int firstFreeIdx;
	if (batchCount == 0)
	{
		firstFreeIdx = 0;
		while (batchIdx >= k)
		{
			Gantry::move(el, k, 0);
			batchIdx -= k;
		}
		return batchIdx;
	}
	else
	{
		firstFreeIdx = firstBatchIdx + k * batchCount;
		if (batchIdx % k != firstBatchIdx % k)
		{
			Gantry::move(el, k, batchIdx);
			for (int i = 0; i < batchCount; i++) //align to the right
				Gantry::move(el, k, firstBatchIdx);
			firstBatchIdx = n - (batchCount + 1) * k - 1;
			int batchesCountToRemove = (n - (batchCount + 1) * k) / k;
			for (int i = 0; i < batchesCountToRemove; i++)	//align to the left
			{
				Gantry::move(el, k, 0);
				firstBatchIdx -= k;
			}
			return firstBatchIdx;
		}
		else
		{
			while (firstFreeIdx != batchIdx)
			{
				Gantry::move(el, k, firstFreeIdx);
				batchIdx -= k;
			}
			return firstBatchIdx;
		}
	}
}

void Sorter2::gantrySort()
{
	int desiredPart;
	int searchedPartIndex;
	int maxBatchesCount = getPossibleBatchesCount(elements, k);
	int currentBatchesCount = 0;
	unsigned int firstBatchIndex = 0;

	int sortLev1, sortLev2;

	while (n - currentBatchesCount * k > 2 * k - 1
			&& currentBatchesCount != maxBatchesCount)
	{
		sortLev1 = areSortedToTheLeft(elements, n - k - 2);
		if (sortLev1 > 0 && sortLev1 <= k - 1)
		{
			sortLev2 = areSortedToTheLeft(elements, n - k - 1);
			if (sortLev2 > sortLev1 && sortLev2 > 0 && sortLev2 <= k - 1)
			{
				desiredPart = sortLev2 + 1;
				Gantry::move(elements, k, n - 2 * k);
				for (int i = 0; i < k; i++)
					Gantry::move(elements, k, n - k - 1);
				if (elements->at(n - 1) == desiredPart)	// if desired in last elem
					Gantry::move(elements, k, n - 2 * k);// get any k from below
				else
				{
					searchedPartIndex = findPartOnTheLeft(elements, n - k - 1,
							desiredPart,
							firstBatchIndex + currentBatchesCount * k - 1);
					searchedPartIndex = findValidIdxToMovePartAtPos(elements,
							searchedPartIndex,
							firstBatchIndex + currentBatchesCount * k - 1,
							n - k, k);
					if (searchedPartIndex > 0)
						Gantry::move(elements, k, searchedPartIndex);
					else
						cout << "Error! cannot find proper element" << endl;
				}
			}
			else
				desiredPart = sortLev1 + 1;
		}
		else
			desiredPart = 1;
		if (findPartOnTheLeft(elements, n - 1, desiredPart, n - k - 2) == -1)
		{
			Gantry::move(elements, k, n - 2 * k);// move before inserting found elem
			if (desiredPart == 1)
				searchedPartIndex = findPartOnTheLeft(elements, n - 1,
						desiredPart,
						firstBatchIndex + currentBatchesCount * k - 1);
			else
				searchedPartIndex = findPartOnTheLeft(elements,
						n - desiredPart - 1, desiredPart,
						firstBatchIndex + currentBatchesCount * k - 1);
			searchedPartIndex = findValidIdxToMovePartAtPos(elements,
					searchedPartIndex,
					firstBatchIndex + currentBatchesCount * k - 1,
					n - desiredPart - 1, k);
			Gantry::move(elements, k, searchedPartIndex);
		}
		while (elements->at(n - k - 1) != desiredPart)
			Gantry::move(elements, k, n - k - 1);
		if (desiredPart == k)
		{
			firstBatchIndex = mvBatchMaxToTheLeft(elements, k,
					currentBatchesCount, firstBatchIndex, n - 2 * k);
			currentBatchesCount++;
		}
		printElements();
	}
	if (currentBatchesCount < maxBatchesCount)
	{
		SystematicFinder sFinder = SystematicFinder(elements, k,
				firstBatchIndex + currentBatchesCount * k);
		sFinder.sortLastBatch();
	}
}

void Sorter2::printElements(void)
{
	printVector(elements);
}
