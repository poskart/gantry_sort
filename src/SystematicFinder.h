/*
 * SystematicFinder.h
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#ifndef SYSTEMATICFINDER_H_
#define SYSTEMATICFINDER_H_

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include "Gantry.h"

using namespace std;

class SystematicFinder
{
public:
	SystematicFinder(vector<int> * elements, int k, int startIndex,
			int leftBatches, Gantry * gPtr);
	virtual ~SystematicFinder();
	unsigned int findAnyBatch(void);
	unsigned int findLeftMatchedBatch(int placesToSortOnTheLeft);
	unsigned int matchLeftBatchOnly(int placesToSortOnTheLeft);
	vector<int> findMoveSequence(unsigned int batchIndex);
	void sortLastBatch();
	void generateTree();
	bool allEmptyAtLevelWithIndex(int index);
	int depthFirstSearch(int startIndex, vector<int> * pattern);
	int breadthFirstSearch(int startIndex, vector<int> * pattern);
	void printInfo(void);
	void printNonEmptyIndices(void);
private:
	int k;
	int p;			// p is number of tree branches - default 2 - binary tree
	int startIndex;
	int batchesToBuild;
	int leftBatchElementsCount;	//0, 1 or 2
	vector<int> * elementsVector;
	vector<vector<int> *> tree;
	vector<long> treeLevelsLastIndices;
	int emptyVectorsCount;
	int nonEmptyVectorsCount;
	long gantryMovesCounter;
	Gantry * gantryPtr;
};

#endif /* SYSTEMATICFINDER_H_ */
