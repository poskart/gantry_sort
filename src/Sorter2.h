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
#include "common.h"
#include "Gantry.h"
#include "SystematicFinder.h"

using namespace std;

class Sorter2
{
public:
	Sorter2(vector<int> * vec, int k, int n);
	virtual ~Sorter2();
	const int areSortedToTheLeft(vector<int> * elements,
			int startIndexIncluded);
	const int findValidIdxToMovePartAtPos(vector<int> * elements,
			int partIndex, int leftBorder, int rightBorder, int k);
	int mvBatchMaxToTheLeft(vector<int> * el, int k, int batchCount,
			int firstBatchIdx, int batchIdx);
	void gantrySort();
	void printElements(void);
private:
	vector<int> * elements;
	int k;
	int n;
};

#endif /* SORTER2_H_ */
