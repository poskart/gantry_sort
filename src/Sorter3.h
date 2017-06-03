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
typedef std::priority_queue<std::pair<int, int>> intPairQueue;

class Sorter3
{
public:
	Sorter3(vector<int> * vec, int k, int n);
	void gantrySort(void);
	void findLeftMatchingPartInTheQueue(intPairQueue * q, int currentPart);
	void printElements(void);
	virtual ~Sorter3();
private:
	vector<int> * elements;
	int k;
	int n;
	Gantry gantry;
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
