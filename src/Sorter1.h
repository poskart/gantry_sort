/*
 * Sorter1.h
 *
 *  Created on: 5 maj 2017
 *      Author: piotr
 */

#ifndef SORTER1_H_
#define SORTER1_H_

#include <vector>
#include <iostream>
#include "common.h"
#include "Gantry.h"
#include "SystematicFinder.h"

using namespace std;
using namespace std::chrono;

class Sorter1
{
public:
	Sorter1(vector<int> * vec, int k, int n);
	virtual ~Sorter1();
	const bool findEveryEachOfK(const vector<int> * elements,
			int startIndex, int partToBeFound);

	void pullKToAlignNextPartToTheLeft(vector<int> * elements,
			int startIndex, int partToBeFound);

	const int findPartStartingBack(vector<int> * elements,
			int shuffleStartIndex, int partNumber);

	const bool findAndShuffleCurrentPart(vector<int> * elements,
			int shuffleStartIndex, int partNumber);

	void gantrySort();

	void gantrySortWatch();

	void printElements(void);

	void cmpltLeftBatchesBySystematic(int currentBatchesCount,
			int maxBatchesCount, int shuffleStartIndex);

	long getGantryMovesCount(void);

	const long getSortingTime(void);

	static long int complexity(long int k, long int n);

private:
	vector<int> * elements;
	int k;
	int n;
	Gantry gantry;
	long prevSortingTime;
};

#endif /* SORTER1_H_ */
