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
	int currentPart;
	int maxBatchesCount = getPossibleBatchesCount(elements, k);
	int currentBatchesCount = 0;
	std::priority_queue<std::pair<int, int>> * queue;

	while (shuffleStartIndex < n - k &&
			currentBatchesCount != maxBatchesCount)
	{
		queue = findLongestSubchains(elements, k, shuffleStartIndex);
		while(true)	// w kolejce są nadającec się podciągi
		{
			shuffleStartIndex += 1%k;
			while(true); //przeiteruj jeśli są dobrze ustawione
		}
		while(true); //przeiteruj jeśli są dobrze ustawione
		delete queue;
		if(currentPart > k)
			currentBatchesCount++;
	}
	printElements();
	if (currentBatchesCount < maxBatchesCount)
	{
		int leftBatches;
		if(elements->at(shuffleStartIndex - 1) != k)
			leftBatches = 1;
		else
			leftBatches = 2;
		SystematicFinder sFinder = SystematicFinder(elements, k,
				shuffleStartIndex, leftBatches, &gantry);
		sFinder.sortLastBatch();
	}
}

void Sorter3::findLeftMatchingPartInTheQueue(intPairQueue * q, int currentPart)
{
}

void Sorter3::printElements(void)
{
	printVector(elements);
}
