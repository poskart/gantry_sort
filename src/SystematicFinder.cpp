/*
 * SystematicFinder.cpp
 *
 *  Created on: 4 maj 2017
 *      Author: piotr
 */

#include "SystematicFinder.h"

SystematicFinder::SystematicFinder(vector<int> * elements, int k,
		int startIndex, int leftBatches, Gantry * gPtr)
{
	this->k = k;
	this->startIndex = startIndex;
	this->batchesToBuild = leftBatches;
	elementsVector = elements;
	gantryPtr = gPtr;
	if(elements->size() - startIndex < k + 2)
	{
		leftBatchElementsCount = k + 2 - (elements->size() - startIndex);
		if(leftBatchElementsCount > 0)
			this->startIndex -= leftBatchElementsCount;
	}
	else if(elements->at(startIndex-1) != k)
		leftBatchElementsCount = k - elements->at(startIndex-1);
	vector<int>::iterator it = elements->begin() + this->startIndex;
	vector<int> * initialVector = new vector<int>(it, elements->end());
	// p is number of tree branches - default 2 - binary tree
	p = initialVector->size() - k;
	tree.push_back(initialVector);
	emptyVectorsCount = 0;
	nonEmptyVectorsCount = 1;
	gantryMovesCounter = 0;
	long pp = 1;
	long sum = 0;
	treeLevelsLastIndices.reserve(22);
	for (int i = 0; i < 30; i++)
	{
		treeLevelsLastIndices.push_back(sum);
		pp *= p;
		sum += pp;
	}
}

SystematicFinder::~SystematicFinder()
{
	vector<vector<int> *>::iterator it;
	for (it = (++(tree.begin())); it != tree.end(); it++)
	{
		if (!(*it)->empty())
			delete *it;
	}
}

/**
 * This method searches the whole tree and return index of first vector
 * which contains any batch sequence.
 */
unsigned int SystematicFinder::findAnyBatch(void)
{
	unsigned int i;
	for (i = 0; i < tree.size(); i++)
		if (!tree.at(i)->empty())
			if (isSetFoundInLastM(tree.at(i), k, 0))
				break;
	return i;
}

/**
 * This method searches the whole tree and return index of first vector
 * which contains batch sequence and which fill up uncompleted batch
 * sequence on the left side.
 */
unsigned int SystematicFinder::findLeftMatchedBatch(int placesToSortOnTheLeft)
{
	unsigned int i;
	for (i = 0; i < tree.size(); i++)
		if (!tree.at(i)->empty())
			if (isSetFoundInLastMmatchedToLeft(tree.at(i), k, 0,
					placesToSortOnTheLeft))
				break;
	return i;
}

/**
 * This method searches the whole tree and return index of first vector
 * which fill up uncompleted batch sequence on the left side.
 */
unsigned int SystematicFinder::matchLeftBatchOnly(int placesToSortOnTheLeft)
{
	unsigned int i;
	for (i = 0; i < tree.size(); i++)
		if (!tree.at(i)->empty())
			if (isSequenceMatchedToTheLeft(tree.at(i), k, 0,
					placesToSortOnTheLeft))
				break;
	return i;
}

/**
 *	This method search sequence of gantry moves which lead to part sequence
 *	at position batchIndex in the tree. It goes from batchIndex node towards
 *	the root of the tree and collects starting position for gantry move.
 */
vector<int> SystematicFinder::findMoveSequence(unsigned int batchIndex)
{
	vector<int> mvSeq;

	if (batchIndex != tree.size())
	{
		int divRest;
		while (batchIndex != 0)
		{
			divRest = batchIndex % p;
			batchIndex = (batchIndex - 1) / p;	// go to parent index
			// make divRest in range: 0,1,...p-1
			if (divRest == 0)
				divRest = p;
			divRest--;
			mvSeq.push_back(divRest);
		}
	}
	return mvSeq;
}

/**
 * This methods generates tree of all possible output part sequences for
 * each possible gantry move sequence. Next it searches for sequence which
 * matches to previous batch or create another part batch or both.
 */
void SystematicFinder::sortLastBatch()
{
	if (tree.size() < 2)
		generateTree();
	unsigned int seqId;
	if(batchesToBuild == 1)
		seqId = matchLeftBatchOnly(leftBatchElementsCount);
	else if (leftBatchElementsCount <= 0)
		seqId = findAnyBatch();
	else
		seqId = findLeftMatchedBatch(leftBatchElementsCount);

	// sort end of the vector with given gantry according to found sequence
	vector<int> mvSeq = findMoveSequence(seqId);
	for (auto rit = mvSeq.crbegin(); rit != mvSeq.crend(); ++rit)
	{
		gantryPtr->move(elementsVector, k, *rit + startIndex);
	}
}

/**
 * This methods generates tree of all possible output part sequences for
 * each possible gantry move sequence.
 */
void SystematicFinder::generateTree()
{
	vector<int> * vecPtr;
	for (unsigned int i = 0; i < tree.size(); i++)
	{
		if (!(tree.at(i)->empty()))
		{
			// Create all children sequences
			for (int j = 0; j < p; j++)
			{
				vecPtr = new vector<int>(*tree[i]);
				vecPtr->reserve(vecPtr->size() + 2 * k);
				gantryPtr->move(vecPtr, k, j);
				gantryMovesCounter++;
				if (depthFirstSearch(p * i + j + 1, vecPtr) >= 0
						|| breadthFirstSearch(p * i + j + 1, vecPtr) >= 0)
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
			for (int j = 0; j < p; j++)
			{
				vecPtr = new vector<int>(0);
				tree.push_back(vecPtr);
				emptyVectorsCount++;
			}
		}
		// If current tree element i is the last element in this tree level
		// check if this level is empty. If so, then tree is completed.
		if (std::find(treeLevelsLastIndices.begin(),
				treeLevelsLastIndices.end(), i) != treeLevelsLastIndices.end())
			if (allEmptyAtLevelWithIndex(i + 1))
				break;
	}
}

/**
 * This method checks if tree level, which starts from 'index' index
 * is empty or not. Returns true if this level has all vectors empty,
 * false otherwise.
 */
bool SystematicFinder::allEmptyAtLevelWithIndex(int index)
{
	for (unsigned int i = index; i < tree.size(); i++)
	{
		if (!tree.at(i)->empty())
			return false;
	}
	return true;
}

/**
 * This method searches tree in depth and looks for vector pointed by pattern.
 * Returns -1 if pattern not found, otherwise returns index of tree node which
 * vector equals to pattern.
 */
int SystematicFinder::depthFirstSearch(int startIndex, vector<int> * pattern)
{
	int i = startIndex;

	do
	{
		i = (i - 1) / p;
		if (*(tree.at(i)) == *pattern)
			return i;
	} while (i != 0);

	return -1;
}

/**
 * This method searches tree in breadth and looks for vector pointed by pattern.
 * Returns -1 if pattern not found, otherwise returns index of tree node which
 * vector equals to pattern.
 */
int SystematicFinder::breadthFirstSearch(int startIndex, vector<int> * pattern)
{
	startIndex--;
	for (; startIndex >= 0; startIndex--)
	{
		if (*(tree.at(startIndex)) == *pattern)
			return true;
	}
	return -1;
}
/**
 * Prints statistics about systematic finder
 */
void SystematicFinder::printInfo(void)
{
	cout << "Tree size: " << tree.size() << endl;
	cout << "Empty elements: " << emptyVectorsCount << endl;
	cout << "Non empty elements: " << nonEmptyVectorsCount << endl;
	for (unsigned int i = 0; i < treeLevelsLastIndices.size(); i++)
	{
		cout << treeLevelsLastIndices.at(i) << "\t";
	}
	cout << endl;
}

void SystematicFinder::printNonEmptyIndices(void)
{
	cout << "Indeksy: " << endl;
	for (unsigned int i = 0; i < tree.size(); i++)
		if (!tree.at(i)->empty())
			cout << i << ", ";
}
