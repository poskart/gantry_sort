/*
 * gantry.h
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#ifndef GANTRY_H_
#define GANTRY_H_

#include <iostream>
#include <vector>
#include "common.h"

using namespace std;

class Gantry
{
public:
	Gantry();
	~Gantry();
	void moveIt(vector<int> * vec, int k, vector<int>::iterator & it);

	void move(vector<int> * vec, int k, int i);

	long getMovesCount(void);
private:
	long gantryMoves;
};



#endif /* GANTRY_H_ */
