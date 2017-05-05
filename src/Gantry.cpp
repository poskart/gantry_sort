#include "Gantry.h"

int Gantry::gantryMoves = 0;

void Gantry::moveIt(vector<int> * vec, vector<int>::iterator & it)
{
	if(vec->end() - it > K)
	{
		vector<int>::iterator it2 = it;
		for(int i = 0; i < K; i++, it2++)
		{
			vec->push_back(*it2);
		}
		vec->erase(it, it2);
	}
}

void Gantry::move(vector<int> * vec, int i)
{
	vector<int>::iterator it = vec->begin() + i;
	moveIt(vec, it);
	gantryMoves++;
}
