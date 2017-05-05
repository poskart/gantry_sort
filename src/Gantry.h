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
	static int gantryMoves;

	static void moveIt(vector<int> * vec, vector<int>::iterator & it);

	static void move(vector<int> * vec, int i);
};



#endif /* GANTRY_H_ */
