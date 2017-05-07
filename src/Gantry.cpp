#include "Gantry.h"

int Gantry::gantryMoves = 0;

void Gantry::moveIt(vector<int> * vec, int k, vector<int>::iterator & it)
{
	if(vec->end() - it > k)
	{
		vector<int>::iterator it2 = it;
		for(int i = 0; i < k; i++, it2++)
		{
			vec->push_back(*it2);
		}
		vec->erase(it, it2);
		gantryMoves++;
	}
}

void Gantry::move(vector<int> * vec, int k, int i)
{
	vector<int>::iterator it = vec->begin() + i;
	moveIt(vec, k, it);
}
