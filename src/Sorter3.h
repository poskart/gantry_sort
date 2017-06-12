/*
 * Sorter3.h
 *
 *  Created on: 1 cze 2017
 *      Author: piotr
 */

#ifndef SORTER3_H_
#define SORTER3_H_

#include <queue>
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include "common.h"
#include "Gantry.h"
#include "SystematicFinder.h"

using namespace std;
using namespace std::chrono;
typedef std::priority_queue<std::pair<int, int>> intPairQueue;

class Sorter3
{
public:
	Sorter3(vector<int> * vec, int k, int n);
	void gantrySort(void);
	std::pair<int, int> findBestFittedSubchain(intPairQueue * q, int desiredPart);
	void removeBatchesBetweenIdcs(vector<int> * elements,
			int startIndex, int partIndex);

	const bool findEveryEachOfK(const vector<int> * elements,
			int startIndex, int partToBeFound);

	void pullKToAlignNextPartToTheLeft(vector<int> * elements,
			int startIndex, int partToBeFound);

	const int findPartStartingBack(vector<int> * elements,
			int shuffleStartIndex, int partNumber);

	const bool findAndShuffleCurrentPart(vector<int> * elements,
			int shuffleStartIndex, int partNumber);

	int shuffleCurrentSubchain(vector<int> * elements,
			int subchainStartIdx, int subchPosition);
	void printElements(void);
	long getGantryMovesCount(void);
	const long getSortingTime(void);
	virtual ~Sorter3();
private:
	vector<int> * elements;
	int k;
	int n;
	Gantry gantry;
	long prevSortingTime;
};

template<typename T> void print_queue(T& q) {
	std::pair<int, int> para;
    while(!q.empty())
    {
    	para = q.top();
        std::cout << "Długość: "<<para.first << " indeks: "<<para.second<<endl;
        q.pop();
    }
    std::cout << '\n';
}

std::priority_queue<std::pair<int, int>> * findLongestSubchains(
		vector<int> * elements, int k, int startIndex);

#endif /* SORTER3_H_ */
