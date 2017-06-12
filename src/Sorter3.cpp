/*
 * Sorter3.cpp
 *
 *  Created on: 1 cze 2017
 *      Author: piotr
 */

#include "Sorter3.h"

Sorter3::Sorter3(vector<int> * vec, int k, int n)
{
	this->elements = new vector<int>(*vec);
	this->elements->reserve(vec->size() + k);
	this->n = n;
	this->k = k;
	this->prevSortingTime = 0;
}

Sorter3::~Sorter3()
{
	delete elements;
}

intPairQueue * findLongestSubchains(
		vector<int> * elements, int k, int startIndex)
{
	int i, j, lmax, n;
	intPairQueue * queue = new std::priority_queue<
			std::pair<int, int>>();
	n = elements->size() - startIndex;

	int** L = new int*[n + 1];
	for (int i = 0; i < n + 1; ++i)
		L[i] = new int[k + 1];

	int * pattern = new int[k];

	for (i = 0; i < k; i++)
		pattern[i] = i + 1;
	for (i = 0; i <= n; i++)
		L[i][0] = 0;
	for (i = 0; i <= k; i++)
		L[0][i] = 0;

	lmax = 0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < k; j++)
		{
			if (elements->at(i + startIndex) != pattern[j])
				L[i + 1][j + 1] = 0;
			else
			{
				L[i + 1][j + 1] = 1 + L[i][j];
				if (L[i + 1][j + 1] > 1)
					queue->push(
							make_pair(L[i + 1][j + 1],
									i - L[i + 1][j + 1] + 1 + startIndex));
				if (L[i + 1][j + 1] > lmax)
				{
					lmax = L[i + 1][j + 1];
					//indeks = i - lmax + 1;
				}
			}
		}
	}
	for (int i = 0; i < n + 1; ++i)
		delete[] L[i];
	delete[] L;

	return queue;
}

void Sorter3::gantrySort(void)
{
	int shuffleStartIndex = 0;
	int desiredPart = 1;
	int maxBatchesCount = getPossibleBatchesCount(elements, k);
	int currentBatchesCount = 0;
	std::priority_queue<std::pair<int, int>> * queue;
	std::pair<int, int> pair;
	int tmp, newElemCount, tmpIdx;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	while (shuffleStartIndex < n - k &&
			currentBatchesCount != maxBatchesCount)
	{
		if(elements->at(shuffleStartIndex) != desiredPart)
		{
			queue = findLongestSubchains(elements, k, shuffleStartIndex);
			pair = findBestFittedSubchain(queue, desiredPart);
			if(pair.first != -1)
			{
				if((pair.second - shuffleStartIndex)%k == 0)
				{
					removeBatchesBetweenIdcs(elements, shuffleStartIndex,
						pair.second);
					newElemCount = pair.first;
				}
				else
				{
					if(pair.second < n - k)
					{
						gantry.move(elements, k, pair.second);
						tmpIdx = n - k;
					}
					else
						tmpIdx = pair.second;
					tmp = shuffleCurrentSubchain(elements, shuffleStartIndex,
							tmpIdx);
					removeBatchesBetweenIdcs(elements, shuffleStartIndex, tmp);
					newElemCount = pair.first > n-tmp ? n-tmp : pair.first;
				}
				shuffleStartIndex += newElemCount;
				desiredPart = (elements->at(shuffleStartIndex-1))%k + 1;
			}
			else
			{
				if (findEveryEachOfK(elements, shuffleStartIndex, desiredPart)
						== true)
				{
					pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
							desiredPart);
				}
				else
				{
					if(findAndShuffleCurrentPart(elements, shuffleStartIndex,
							desiredPart) == false)
						return;
					pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex,
							desiredPart);
				}
				shuffleStartIndex++;
				desiredPart = desiredPart%k + 1;
			}
			if (shuffleStartIndex >= n - k)
								break;
			if(desiredPart == 1)
				currentBatchesCount++;
			delete queue;
		}
		else
		{
			shuffleStartIndex++;
			desiredPart = desiredPart%k + 1;
			if(desiredPart == 1)
				currentBatchesCount++;
		}
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	prevSortingTime = duration_cast<microseconds>( t2 - t1 ).count();
}

/**
 * This method pull with gantry all elements which are between currently
 * ordered index and index of the found part to align found part to the left.
 */
void Sorter3::removeBatchesBetweenIdcs(vector<int> * elements,
		int startIndex, int partIndex)
{
	int batchesToRemove = (partIndex - startIndex)/k;
	for(int i = 0; i < batchesToRemove; i++)
	{
		gantry.move(elements, k, startIndex);
	}
}

/**
 * This method check if in the elements vector there is partToBeFound part
 * every each of k.
 * Returns true if partToBeFound was found, false otherwise.
 */
const bool Sorter3::findEveryEachOfK(const vector<int> * elements,
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
void Sorter3::pullKToAlignNextPartToTheLeft(vector<int> * elements,
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
const int Sorter3::findPartStartingBack(vector<int> * elements,
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
const bool Sorter3::findAndShuffleCurrentPart(vector<int> * elements,
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

std::pair<int, int> Sorter3::findBestFittedSubchain(intPairQueue * q, int desiredPart)
{
	std::pair<int, int> pair;
	for(unsigned int i = 0; i < q->size(); i++)
	{
		pair = q->top();
		if(elements->at(pair.second) != desiredPart)
			q->pop();
		else
			return pair;
	}
	return std::make_pair<int, int>(-1, -1);
}

int Sorter3::shuffleCurrentSubchain(vector<int> * elements,
		int shuffleStartIndex, int subchPosition)
{
	/*
	 * Find exact position where current part should be set
	 * to make space between it and shuffleStartIndex divisible by k.
	 */
	int targetSubchPosition, movesCount;
	for (targetSubchPosition = n-k-1; targetSubchPosition != n; targetSubchPosition++)
	{
		if (((targetSubchPosition - shuffleStartIndex) % k) == 0)
			break;
	}
	if(targetSubchPosition == (n - k - 1))
		movesCount = n-subchPosition;
	else if(targetSubchPosition - subchPosition >= 0)
		movesCount = targetSubchPosition - subchPosition;
	else
		movesCount = targetSubchPosition - subchPosition + k + 1;
	/*
	 * Move circular k+1 last elements to set partnumber in proper position.
	 */
	for(int i = 0; i < movesCount; i++)
	{
		gantry.move(elements, k, n - k - 1);
	}
	return targetSubchPosition;
}

void Sorter3::printElements(void)
{
	printVector(elements);
}

long Sorter3::getGantryMovesCount(void)
{
	return gantry.getMovesCount();
}

const long Sorter3::getSortingTime(void)
{
	return prevSortingTime;
}
