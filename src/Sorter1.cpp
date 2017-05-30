/*
 * Sorter1.cpp
 *
 *  Created on: 5 maj 2017
 *      Author: piotr
 */

#include "Sorter1.h"

Sorter1::Sorter1(vector<int> * vec, int k, int n)
{
	this->elements = new vector<int>(*vec);
	this->elements->reserve(vec->size() + k);
	this->n = n;
	this->k = k;
}

Sorter1::~Sorter1()
{
	delete elements;
}

const bool Sorter1::findEveryEachOfK(const vector<int> * elements,
		int startIndex, int partToBeFound)
{
	while (startIndex < n - 1)
	{
		if (elements->at(startIndex) == partToBeFound)
			return true;
		startIndex += k;
	}
	return false;
}

void Sorter1::pullKToAlignNextPartToTheLeft(vector<int> * elements,
		int startIndex, int partToBeFound)
{
	while (elements->at(startIndex) != partToBeFound)
	{
		gantry.move(elements, k, startIndex);
	}
}

const int Sorter1::findPartStartingBack(vector<int> * elements,
		int shuffleStartIndex, int partnumber)
{
	int currentIndex = n - k;
	while (currentIndex >= shuffleStartIndex)
	{
		for (int i = k - 1; i >= 0; i--)
		{
			if (elements->at(currentIndex + i) == partnumber)
				return currentIndex;
		}
		currentIndex -= k;
	}
	return -1;
}

const bool Sorter1::findAndShuffleCurrentPart(vector<int> * elements,
		int shuffleStartIndex, int partnumber)
{
	int index = findPartStartingBack(elements, shuffleStartIndex, partnumber);
	if (index < 0)
		return false;
	else if (index < n - k - 1)
	{
		gantry.move(elements, k, index);
		index = n - k;
	}
	/*
	 * Find exact position where current part should be set
	 * to make space between it and shuffleStartIndex divisible by k.
	 */
	int targetPartPosition;
	for (int i = 0; i < k; i++)
	{
		if (((index + i - shuffleStartIndex) % k) == 0)
		{
			targetPartPosition = index + i;
			break;
		}
	}
	/*
	 * Move circular k+1 last elements to set partnumber in proper position.
	 */
	while (elements->at(targetPartPosition) != partnumber)
	{
		gantry.move(elements, k, n - k - 1);
	}
	return true;
}

void Sorter1::gantrySort()
{
	int shuffleStartIndex = 0;
	int currentPart;
	bool endFlag = false;
	int maxBatchesCount = getPossibleBatchesCount(elements, k);
	int currentBatchesCount = 0;

	while (shuffleStartIndex < n - k - 2 && endFlag == false
			&& currentBatchesCount != maxBatchesCount)
	{
		for (currentPart = 1; currentPart <= k; currentPart++)
		{
			if (findEveryEachOfK(elements, shuffleStartIndex, currentPart)
					== true)
			{
				if (elements->at(shuffleStartIndex) != currentPart)
					pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
							currentPart);
			}
			else
			{
				if (findAndShuffleCurrentPart(elements, shuffleStartIndex,
						currentPart))
				{
					pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
							currentPart);
				}
//				else
//				{
//					endFlag = true;
//					break;
//				}
			}
			shuffleStartIndex++;
			if (shuffleStartIndex >= n - k - 2)
				break;
		}
		if(currentPart > k)
			currentBatchesCount++;
	}
	if (currentBatchesCount < maxBatchesCount)
	{
		SystematicFinder sFinder = SystematicFinder(elements, k,
				shuffleStartIndex, &gantry);
		sFinder.sortLastBatch();
	}
}

void Sorter1::printElements(void)
{
	printVector(elements);
}

long Sorter1::getGantryMovesCount(void)
{
	return gantry.getMovesCount();
}
