//============================================================================
// Name        : AAL.cpp
// Author      : Piotr Poskart
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <random>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "SystematicFinder.h"

using namespace std;

vector<int> generator(int k, int n)
{
	srand(time(NULL));
	vector<int> testTable;
	for (int i = 0; i < n; i++)
	{
		testTable.push_back(rand()%K + 1);
	}
	return testTable;
}

vector<int> evenlyGenerator(int k, int n)
{
	vector<int> vec;
	int setCount = n/k;
	for(int i = 0; i < setCount; i++)
	{
		for(int j = 1; j <= k; j++)
			vec.push_back(j);
	}
	srand(time(NULL));
	// Fill remaining places
	for(int i = 0; i < (N - setCount*k); i++)
	{
		vec.push_back(rand()%K + 1);
	}
	std::random_shuffle(vec.begin(), vec.end());
	return vec;
}

const bool findEveryEachOfK(const vector<int> * elements, int startIndex, int partToBeFound)
{
	while(startIndex < N - 1)
	{
		if(elements->at(startIndex) == partToBeFound)
			return true;
		startIndex += K;
	}
	return false;
}

void pullKToAlignNextPartToTheLeft(vector<int> * elements, int startIndex, int partToBeFound)
{
	while(elements->at(startIndex) != partToBeFound)
	{
		Gantry::move(elements, startIndex);
	}
}

const int findPartStartingBack(vector<int> * elements, int shuffleStartIndex, int partNumber)
{
	int currentIndex = N - K;
	while(currentIndex >= shuffleStartIndex)
	{
		for(int i = K - 1; i >= 0; i--)
		{
			if(elements->at(currentIndex + i) == partNumber)
				return currentIndex;
		}
		currentIndex -= K;
	}
	return -1;
}

const bool findAndShuffleCurrentPart(vector<int> * elements, int shuffleStartIndex, int partNumber)
{
	int index = findPartStartingBack(elements, shuffleStartIndex, partNumber);
	if(index < 0)
		return false;
	else if(index < N - K - 1)
	{
		Gantry::move(elements, index);
		index = N - K;
	}
	/*
	 * Find exact position where current part should be set
	 * to make space between it and shuffleStartIndex divisible by K.
	 */
	int targetPartPosition;
	for(int i = 0; i < K; i++)
	{
		if(((index + i - shuffleStartIndex) % K) == 0)
		{
			targetPartPosition = index + i;
			break;
		}
	}
	/*
	 * Move circular K+1 last elements to set partNumber in proper position.
	 */
	while(elements->at(targetPartPosition) != partNumber)
	{
		Gantry::move(elements, N - K - 1);
	}
	return true;
}

void gantrySort1(vector<int> * elements)
{
	int shuffleStartIndex = 0;
	int currentPart;
	bool endFlag = false;
	while(shuffleStartIndex < N && endFlag == false)
	{
		if(shuffleStartIndex < N - K - 2)
		{

			for(currentPart = 1; currentPart <= K; currentPart++)
			{
				if(findEveryEachOfK(elements, shuffleStartIndex, currentPart) == true)
				{
					if(elements->at(shuffleStartIndex) != currentPart)
						pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex, currentPart);
				}
				else
				{
					if(findAndShuffleCurrentPart(elements, shuffleStartIndex, currentPart))
					{
						pullKToAlignNextPartToTheLeft(elements, shuffleStartIndex, currentPart);
					}
					else
					{
						endFlag = true;
						break;
					}
				}
				shuffleStartIndex++;
			}
		}
		else
		{
			while(!isSortedFromXtoN(elements, N - K - 2))
			{
				if(are2PartsInCorrectOrder(elements, N - K - 2, N - K - 1))
				{
					Gantry::move(elements, N - K - 2);
				}
				else
				{
					Gantry::move(elements, N - K - 1);
				}
			}
			endFlag = true;
		}
	}
}

int main()
{
	vector<int> vec;
	vec = evenlyGenerator(K, N);

	printVector(&vec);
	fflush(stdout);

	vec.reserve(N + 2 * K);

	//gantrySort1(&vec);

	int shuffleStartIndex = 0;
	int currentPart;
	bool endFlag = false;

	while(shuffleStartIndex < N - K - 2 && endFlag == false)
	{

		for(currentPart = 1; currentPart <= K; currentPart++)
		{
			if(findEveryEachOfK(&vec, shuffleStartIndex, currentPart) == true)
			{
				if(vec.at(shuffleStartIndex) != currentPart)
					pullKToAlignNextPartToTheLeft(&vec, shuffleStartIndex, currentPart);
			}
			else
			{
				if(findAndShuffleCurrentPart(&vec, shuffleStartIndex, currentPart))
				{
					pullKToAlignNextPartToTheLeft(&vec, shuffleStartIndex, currentPart);
				}
				else
				{
					endFlag = true;
					break;
				}
			}
			shuffleStartIndex++;
			if(shuffleStartIndex >= N - K - 2)
				break;
		}
	}

	K = 6;
	N = 8;
	vector<int> test = {4, 2, 1, 6, 1, 3, 5, 7};
	test.reserve(7 + 2 * K);
	SystematicFinder finder(K, test);
	finder.generateTree();
	finder.printInfo();
	finder.printNonEmptyIndices();
	vector<int> mvSeq;
	mvSeq = finder.findMoveSequence();
	if(!mvSeq.empty())
	{
		cout<<"\nSekwencja ruchów: (rozmiar wektora: "<<mvSeq.size()<<")"<<endl;
	    for (auto rit = mvSeq.crbegin(); rit != mvSeq.crend(); ++rit)
	    	cout<< *rit<<endl;
	}

	cout<<"\nPosortowane:"<<endl;
	printVector(&vec);
	cout<<"\nLiczba ruchów suwnicy: "<<Gantry::gantryMoves<<endl;

	cout<<"Koniec..."<<endl;

	return 0;
}
