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
	this->countingTable = nullptr;
}

Sorter2::~Sorter2()
{
	if(countingTable != nullptr)
		delete [] countingTable;
	delete elements;
}

const int Sorter2::areSortedToTheLeft(vector<int> * elements,
		unsigned int startIndexIncluded)
{
	if (startIndexIncluded >= elements->size())
	{
		cout << "Error in Sorter2::areSortedToTheLeft()" << endl;
		return 0;
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
		int partIndex, int leftBorderExcl, int rightBorderExcl, int k)
{
	if (partIndex + k - 1 < rightBorderExcl)
	{
		if (partIndex > leftBorderExcl)
			return partIndex;
		else
			return -1;
	}
	else
	{
		while (partIndex + k - 1 >= rightBorderExcl)
			partIndex--;
		if (partIndex > leftBorderExcl)
			return partIndex;
		else
			return -1;
	}
}

int Sorter2::mvBatchMaxToTheLeft(vector<int> * el, int k, int batchCount,
		int firstBatchIdx, int batchIdx)
{
	int firstFreeIdx;
	int unalignedPartsCount = (n - 2 * k) % k;
	if (batchCount == 0)
	{
		/**
		 * omit elements that are blocked on the left, they have been
		 * already placed there as unused
		 */
		firstFreeIdx = 0;
		while (batchIdx >= k)
		{
			gantry.move(el, k, unalignedPartsCount);
			batchIdx -= k;
		}
		return batchIdx;
	}
	else
	{
		firstFreeIdx = firstBatchIdx + k * batchCount;
		if (batchIdx % k != firstBatchIdx % k)
		{
			gantry.move(el, k, batchIdx);
			for (int i = 0; i < batchCount; i++) //align to the right
				gantry.move(el, k, firstBatchIdx);
			firstBatchIdx = n - (batchCount + 1) * k - 1;
			int batchesCountToRemove = (n - (batchCount + 1) * k) / k;
			for (int i = 0; i < batchesCountToRemove; i++)	//align to the left
			{
				gantry.move(el, k, unalignedPartsCount);
				firstBatchIdx -= k;
			}
		}
		else
		{
			while (firstFreeIdx != batchIdx)
			{
				gantry.move(el, k, firstFreeIdx);
				batchIdx -= k;
			}
		}
		return firstBatchIdx;
	}
}

void Sorter2::gantrySort()
{
	int desiredPart;
	int searchedPartIndex;
	int maxBatchesCount = getPossibleBatchesCount();
	int currentBatchesCount = 0;
	unsigned int firstBatchIndex = 0;
	int sortLev1, sortLev2;

	prepareVectorHead(maxBatchesCount);
	while (n - currentBatchesCount * k > 2 * k - 1
			&& currentBatchesCount != maxBatchesCount)
	{
		sortLev1 = areSortedToTheLeft(elements, n - k - 2);
		if (sortLev1 <= k - 1)
		{
				desiredPart = sortLev1 + 1;
		}
		sortLev2 = areSortedToTheLeft(elements, n - k - 1);
		if ((sortLev2 > sortLev1 || sortLev1 == 4) && sortLev2 > 0 && sortLev2 <= k - 1)
		{
			desiredPart = sortLev2 + 1;
			gantry.move(elements, k, n - 2 * k);
			for (int i = 0; i < k; i++)
				gantry.move(elements, k, n - k - 1);
			if (elements->at(n - 1) == desiredPart)	// if desired in last elem
				gantry.move(elements, k, n - 2 * k);// get any k from below
			else
			{
				searchedPartIndex = findPartOnTheLeft(elements, n - desiredPart - 1,
						desiredPart,
						firstBatchIndex + currentBatchesCount * k);
				searchedPartIndex = findValidIdxToMovePartAtPos(elements,
						searchedPartIndex,
						firstBatchIndex + currentBatchesCount * k - 1,
						n - desiredPart, k);
				if (searchedPartIndex > 0)
					gantry.move(elements, k, searchedPartIndex);
				else
					cout << "Error! cannot find proper element" << endl;
			}
		}
		else if((sortLev1 == 0 || sortLev1 == 4) && sortLev2 == 0)
			desiredPart = 1;
		// if no desiredPart in range n-k-1 : n-1
		if (findPartOnTheLeft(elements, n - 1, desiredPart, n - k - 1) == -1)
		{
			gantry.move(elements, k, n - 2 * k);// move before inserting found elem
			if (desiredPart == 1)
				searchedPartIndex = findPartOnTheLeft(elements, n - 1,
						desiredPart,
						firstBatchIndex + currentBatchesCount * k);
			else
				searchedPartIndex = findPartOnTheLeft(elements,
						n - desiredPart - 1, desiredPart,
						firstBatchIndex + currentBatchesCount * k);
			searchedPartIndex = findValidIdxToMovePartAtPos(elements,
					searchedPartIndex,
					firstBatchIndex + currentBatchesCount * k - 1,
					n - desiredPart, k);
			gantry.move(elements, k, searchedPartIndex);
		}
		while (elements->at(n - k - 1) != desiredPart)
			gantry.move(elements, k, n - k - 1);
		if (desiredPart == k)
		{
			firstBatchIndex = mvBatchMaxToTheLeft(elements, k,
					currentBatchesCount, firstBatchIndex, n - 2 * k);
			currentBatchesCount++;
		}
//		printElements();
	}
	printElements();
	if (currentBatchesCount < maxBatchesCount)
	{
		int startIndex = firstBatchIndex + currentBatchesCount * k;
		SystematicFinder sFinder = SystematicFinder(elements, k,
				startIndex, 2, &gantry);
		sFinder.sortLastBatch();
	}
}

void Sorter2::printElements(void)
{
	printVector(elements);
}

long Sorter2::getGantryMovesCount(void)
{
	return gantry.getMovesCount();
}

unsigned int Sorter2::getPossibleBatchesCount(void)
{
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
	return minPartsCount;
}

vector<int> Sorter2::getPartsWithoutBatch(int maxBatchesCount)
{
	vector<int> unconnectedParts;
	int redundantElementsCount;
	for(int i = 1; i < k + 1; i++)
	{
		redundantElementsCount = 0;
		while(countingTable[i] - redundantElementsCount > maxBatchesCount)
		{
			unconnectedParts.push_back(i);
			redundantElementsCount++;
		}
	}
	return unconnectedParts;
}

const bool Sorter2::areNeededPartsBlockedOnTheLeft(int maxBatchesCount, int blockedElemCount)
{
	for(int i = 0; i < blockedElemCount; i++)
	{
		if(countingTable[elements->at(i)] <= maxBatchesCount)
			return true;
		countingTable[elements->at(i)]--;
	}
	return false;
}

void Sorter2::mvSparePartsToTheLeft(vector<int> * spareParts, int blockedElemCount)
{
	int shuffleStartIndex = 0;
	vector<int>::iterator currentPartIt;
	for(int i = 0; i < blockedElemCount; i++)
	{
		currentPartIt = std::find(spareParts->begin(), spareParts->end(), elements->at(i));
		if(currentPartIt == spareParts->end())
		{
			currentPartIt = findEveryEachOfK(elements, shuffleStartIndex, spareParts);
			if (currentPartIt != spareParts->end())
			{
				pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
					*currentPartIt);
				spareParts->erase(currentPartIt);
			}
			else
			{
				currentPartIt = spareParts->begin();
				findAndShuffleCurrentPart(elements, shuffleStartIndex,
						*currentPartIt);
				pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
						*currentPartIt);
				spareParts->erase(currentPartIt);
			}
		}
		else
		{
			spareParts->erase(currentPartIt);
		}
		shuffleStartIndex++;
	}
}

vector<int>::iterator Sorter2::findEveryEachOfK(vector<int> * elements,
		int startIndex, vector<int> * possibleElements)
{
	vector<int>::iterator it;
	while (startIndex < n - 1)
	{
		it = std::find(possibleElements->begin(), possibleElements->end(), elements->at(startIndex));
		if (it != possibleElements->end())
			return it;
		startIndex += k;
	}
	return possibleElements->end();
}

void Sorter2::pullKToAlignNextPartToTheLeft(vector<int> * elements,
		int startIndex, int partToBeFound)
{
	while (elements->at(startIndex) != partToBeFound)
	{
		gantry.move(elements, k, startIndex);
	}
}

const int Sorter2::findPartStartingBack(vector<int> * elements,
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

const bool Sorter2::findAndShuffleCurrentPart(vector<int> * elements,
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

void Sorter2::prepareVectorHead(int maxBatchesCount)
{
	int blockedElemCount = (n - 2 * k) % k;
	if(blockedElemCount != 0)
	{
		vector<int> freeParts = getPartsWithoutBatch(maxBatchesCount);
		if(areNeededPartsBlockedOnTheLeft(maxBatchesCount, blockedElemCount))
			mvSparePartsToTheLeft(&freeParts, blockedElemCount);
	}
}
