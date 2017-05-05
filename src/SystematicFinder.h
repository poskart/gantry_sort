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
	SystematicFinder(int k, vector<int> tip);
	virtual ~SystematicFinder();
	vector<int> findMoveSequence();
	void generateTree();
	bool allEmptyAtLevelWithIndex(int index);
	int depthFirstSearch(int startIndex, vector<int> * pattern);
	int breadthFirstSearch(int startIndex, vector<int> * pattern);
	void printInfo(void);
	void printNonEmptyIndices(void);
private:
	int k;
	int p;
	vector<int> initialVector;
	vector<vector<int> *> tree;
	vector<long> treeLevelsLastIndices;
	int emptyVectorsCount;
	int nonEmptyVectorsCount;
	long gantryMovesCounter;
};

#endif /* SYSTEMATICFINDER_H_ */
