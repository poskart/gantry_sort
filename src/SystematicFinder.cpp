/*
 * SystematicFinder.cpp
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#include "SystematicFinder.h"

SystematicFinder::SystematicFinder(int k, vector<int> tip)
{
	this->k = k;
	this->initialVector = tip;
	this->p = initialVector.size() - k;
	tree.push_back(&initialVector);
	this->emptyVectorsCount = 0;
	this->nonEmptyVectorsCount = 1;
	this->gantryMovesCounter = 0;
	long pp = 1;
	long sum = 0;
	for(int i = 0; i < 22; i++)
	{
		treeLevelsLastIndices.push_back(sum);
		pp *= p;
		sum += pp;
	}
}

SystematicFinder::~SystematicFinder()
{
	vector<vector<int> * >::iterator it;
	for(it = tree.begin(); it != tree.end(); it++)
	{
		if(*it != nullptr)
			delete *it;
	}
}

vector<int> SystematicFinder::findMoveSequence()
{
	vector<int> mvSeq;
	int i;
	for(i = 0; i < tree.size(); i++)
	{
		if(!tree.at(i)->empty())
		{
			if(isSetFoundInLastM(tree.at(i), 0))
				break;
		}
	}
	if(i != tree.size())
	{
		int divRest;
		while(i != 0)
		{
			divRest = i % p;
			i = (i - 1) / p;	// go to parent index
			if(divRest == 0)
				divRest = p;
			divRest--;
			mvSeq.push_back(divRest);
		}
	}
	return mvSeq;
}

void SystematicFinder::generateTree()
{
	vector<int> * vecPtr;
	for(unsigned int i = 0; i < tree.size(); i++)
	{
		if(!(tree.at(i)->empty()))
		{
			for(int j = 0; j < p; j++)
			{
				vecPtr = new vector<int>(*tree[i]);
				vecPtr->reserve(vecPtr->size() + 2*K);
				Gantry::move(vecPtr, j);
				gantryMovesCounter++;
				if(depthFirstSearch(p*i + j + 1, vecPtr) >= 0 || breadthFirstSearch(p*i + j + 1, vecPtr) >= 0)
				{
					vecPtr->clear();
					vecPtr->reserve(0);
					emptyVectorsCount++;
				}
				else
					nonEmptyVectorsCount++;
				tree.push_back(vecPtr);
			}
		}
		else
		{
			for(int j = 0; j < p; j++)
			{
				vecPtr = new vector<int>(0);
				tree.push_back(vecPtr);
				emptyVectorsCount++;
			}
		}

		if(std::find(treeLevelsLastIndices.begin(), treeLevelsLastIndices.end(), i) != treeLevelsLastIndices.end())
			if(allEmptyAtLevelWithIndex(i + 1))
				break;
	}
}

bool SystematicFinder::allEmptyAtLevelWithIndex(int index)
{
	for(unsigned int i = index; i < tree.size(); i++)
	{
		if(!tree.at(i)->empty())
			return false;
	}
	return true;
}

int SystematicFinder::depthFirstSearch(int startIndex, vector<int> * pattern)
{
	int i = startIndex;

	do
	{
		i = (i - 1) / p;
		if(*(tree.at(i)) == *pattern)
			return i;
	}while(i != 0);

	return -1;
}

int SystematicFinder::breadthFirstSearch(int startIndex, vector<int> * pattern)
{
	startIndex--;
	for(; startIndex >= 0; startIndex--)
	{
		if(*(tree.at(startIndex)) == *pattern)
			return true;
	}
	return -1;
}

void SystematicFinder::printInfo(void)
{
	cout<<"Tree size: "<<tree.size()<<endl;
	cout<<"Empty elements: "<<emptyVectorsCount<<endl;
	cout<<"Non empty elements: "<<nonEmptyVectorsCount<<endl;
	for(unsigned int i = 0; i < treeLevelsLastIndices.size(); i++)
	{
		cout<<treeLevelsLastIndices.at(i)<<"\t";
	}
	cout<<endl;
}

void SystematicFinder::printNonEmptyIndices(void)
{
	cout<<"Indeksy: "<<endl;
	for(unsigned int i = 0; i < tree.size(); i++)
		if(!tree.at(i)->empty())
			cout<<i<<", ";
}
