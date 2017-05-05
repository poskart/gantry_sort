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

using namespace std;

extern int K;
extern int N;

const bool are2PartsInCorrectOrder(vector<int> * elements, int firstPosition, int secPosition);

bool isSetFound(vector<int> * vec, int index);

bool isSetFoundInLastM(vector<int> * vec, int startIndex);

const bool isSortedFromXtoN(vector<int> * elements, int startIndex);

const bool isSortedFromXtoN(vector<int> * elements, int startIndex, int NN);

void printVector(vector<int> * vec);

#endif /* COMMON_H_ */
