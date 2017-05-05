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

unsigned int getPossibleBatchesCount(vector<int> * elements, int k);

inline const bool are2PartsInCorrectOrder(vector<int> * elements, int k,
		int firstPosition, int secPosition);

bool isSetFound(vector<int> * vec, int k, int index);

bool isSetFoundInLastM(vector<int> * vec, int k, int startIndex);

const bool isSortedFromXtoN(vector<int> * elements, int k, int startIndex);

const bool isSortedFromXtoN(vector<int> * elements, int k, int startIndex,
		int NN);

void printVector(vector<int> * vec);

#endif /* COMMON_H_ */
