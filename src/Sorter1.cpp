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
	this->prevSortingTime = 0;
}

Sorter1::~Sorter1()
{
	delete elements;
}
/**
 * This method check if in the elements vector there is partToBeFound part
 * every each of k.
 * Returns true if partToBeFound was found, false otherwise.
 */
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
/**
 * This method pull with gantry all elements which are between currently
 * ordered index and index of the found part to align found part to the left.
 */
void Sorter1::pullKToAlignNextPartToTheLeft(vector<int> * elements,
		int startIndex, int partToBeFound)
{
	while (elements->at(startIndex) != partToBeFound)
	{
		gantry.move(elements, k, startIndex);
	}
}
/**
 * This method searches for part partNumber starting from the
 * right end and returns index of the found part.
 */
const int Sorter1::findPartStartingBack(vector<int> * elements,
		int shuffleStartIndex, int partNumber)
{
	for (int i = n - 1; i > shuffleStartIndex; i--)
	{
		if (elements->at(i) == partNumber)
		{
			while (i >= n - k)
				i--;
			return i;
		}
	}
	return -1;
}

/**
 * This method searches for part partNumber, moves it to the last k
 * elements if is not there, and shuffle this elements so that partNumber
 * is at position (index) between which and current shuffle index are elements
 * which amount is divisible by k.
 */
const bool Sorter1::findAndShuffleCurrentPart(vector<int> * elements,
		int shuffleStartIndex, int partnumber)
{
	int index = findPartStartingBack(elements, shuffleStartIndex, partnumber);
	if (index < 0)
		return false;
	else if (index < n - k - 1)
		gantry.move(elements, k, index);

	index = n - k;
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

/**
 * This method sorts elements vector using gantry sort algorithm 1
 */
void Sorter1::gantrySort()
{
	int shuffleStartIndex = 0;
	int desiredPart;
	int maxBatchesCount = getPossibleBatchesCount(elements, k);
	int currentBatchesCount = 0;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	while (shuffleStartIndex < n - k && currentBatchesCount != maxBatchesCount)
	{
		for (desiredPart = 1; desiredPart <= k; desiredPart++)
		{
			if (findEveryEachOfK(elements, shuffleStartIndex, desiredPart)
					== true)
			{
				if (elements->at(shuffleStartIndex) != desiredPart)
					pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
							desiredPart);
			}
			else
			{
				findAndShuffleCurrentPart(elements, shuffleStartIndex,
						desiredPart);
				pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
						desiredPart);
			}
			shuffleStartIndex++;
			if (shuffleStartIndex >= n - k)
				break;
		}
		if (desiredPart > k)
			currentBatchesCount++;
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	prevSortingTime = duration_cast<microseconds>( t2 - t1 ).count();
//	printElements();
//	cmpltLeftBatchesBySystematic(currentBatchesCount, maxBatchesCount,
//			shuffleStartIndex);

}

void Sorter1::cmpltLeftBatchesBySystematic(int currentBatchesCount,
		int maxBatchesCount, int shuffleStartIndex)
{
	if (currentBatchesCount < maxBatchesCount)
	{
		int leftBatches;
		if (elements->at(shuffleStartIndex - 1) != k)
			leftBatches = 1;
		else
			leftBatches = 2;
		SystematicFinder sFinder = SystematicFinder(elements, k,
				shuffleStartIndex, leftBatches, &gantry);
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

const long Sorter1::getSortingTime(void)
{
	return prevSortingTime;
}
