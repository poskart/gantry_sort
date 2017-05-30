/*
 * common.h
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int findPartOnTheRight(vector<int> * elements, unsigned int startIndex,
		int partNumber);

const int findPartOnTheLeft(vector<int> * elements, unsigned int startIdxIncl,
		int partNumber, unsigned int leftBorderIdxIncl = 0);

inline const bool are2PartsInCorrectOrder(vector<int> * elements, int k,
		int firstPosition, int secPosition);

unsigned int getPossibleBatchesCount(vector<int> * elements, int k);

bool isSetFound(vector<int> * vec, int k, int index);

bool isSetFoundInLastM(vector<int> * vec, int k, int startIndex);

bool isSetFoundInLastMmatchedToLeft(vector<int> * vec, int k, int startIndex,
		int placesOnTheLeftToSort);

const bool isSortedFromXtoN(vector<int> * elements, int k, unsigned int startIndex);

const bool isSortedFromXtoN(vector<int> * elements, int k, int startIndex,
		int NN);

void printVector(vector<int> * vec);

#endif /* COMMON_H_ */
