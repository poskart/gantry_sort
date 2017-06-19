/*
 * Sorter2.h
 *
 *  Created on: 5 maj 2017
 *      Author: piotr
 */

#ifndef SORTER2_H_
#define SORTER2_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include "common.h"
#include "Gantry.h"
#include "SystematicFinder.h"

using namespace std;
using namespace std::chrono;
class Sorter2
{
public:
	Sorter2(vector<int> * vec, int k, int n);
	virtual ~Sorter2();
	const int areSortedToTheLeft(vector<int> * elements,
			unsigned int startIndexIncluded);
	const int findValidIdxToMovePartAtPos(vector<int> * elements,
			int partIndex, int leftBorder, int rightBorder, int k);
	int mvBatchMaxToTheLeft(vector<int> * el, int k, int batchCount,
			int firstBatchIdx, int batchIdx);
	void gantrySort();
	void gantrySortWatch();
	void cmpltLeftBatchesBySystematic(int currentBatchesCount,
			int maxBatchesCount, int firstBatchIndex);
	void printElements(void);
	long getGantryMovesCount(void);
	unsigned int getPossibleBatchesCount(void);
	vector<int> getPartsWithoutBatch(int maxBatchesCount);
	const bool areNeededPartsBlockedOnTheLeft(int maxBatchesCount, int blockedElemCount);
	void mvSparePartsToTheLeft(vector<int> * spareParts, int blockedElemCount);
	vector<int>::iterator findEveryEachOfK(vector<int> * elements,
			int startIndex, vector<int> * possibleElements);
	void pullKToAlignNextPartToTheLeft(vector<int> * elements,
			int startIndex, int partToBeFound);
	const int findPartStartingBack(vector<int> * elements,
			int shuffleStartIndex, int partnumber);
	const bool findAndShuffleCurrentPart(vector<int> * elements,
			int shuffleStartIndex, int partnumber);
	void prepareVectorHead(int maxBatchesCount);
	const long getSortingTime(void);
	static long int complexity(long int k, long int n);
private:
	vector<int> * elements;
	int * countingTable;
	int k;
	int n;
	Gantry gantry;
	long prevSortingTime;
};

#endif /* SORTER2_H_ */
