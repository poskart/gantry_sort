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
#include "Sorter1.h"
#include "Sorter2.h"

using namespace std;

//#define CHECK 1

vector<int> generator(int k, int n)
{
	srand(time(NULL));
	vector<int> testTable;
	for (int i = 0; i < n; i++)
	{
		testTable.push_back(rand()%k + 1);
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
	for(int i = 0; i < (n - setCount*k); i++)
	{
		vec.push_back(rand()%k + 1);
	}
	std::random_shuffle(vec.begin(), vec.end());
	return vec;
}

int main()
{
	int K = 4;
	int N = 29;
	vector<int> vec;
	//vec = evenlyGenerator(K, N);

#ifndef CHECK
	for(K = 4; K < 7; K++)
	{
		for(N = 20; N < 40; N++)
		{
			vec = generator(K, N);
			cout<<"N = "<<N<<", K = "<<K<<endl;
			printHistogram(&vec, K);

			printVector(&vec);cout<<endl;
			fflush(stdout);
			vec.reserve(N + 2 * K);

			cout<<"Sorter1:"<<endl;
			Sorter1 sorter1(&vec, K, N);
			sorter1.gantrySort();
			sorter1.printElements();
			cout<<"\nLiczba ruchów suwnicy: "<<sorter1.getGantryMovesCount()<<endl;


			cout<<"Sorter2:"<<endl;
			Sorter2 sorter2 = Sorter2(&vec, K, N);
			sorter2.gantrySort();
			sorter2.printElements();
			cout<<"\nLiczba ruchów suwnicy: "<<sorter2.getGantryMovesCount()<<endl;
		}
	}
#else
	K = 7;
	N = 29;
	vec = {6, 1, 2, 3, 2, 2, 1, 2, 5, 3, 5, 3, 1, 7, 4, 5, 6, 6, 1, 6, 4, 4, 6, 7, 4, 1, 4, 5, 7};
	printHistogram(&vec, K);maxBatchesCount - currentBatchesCount;
	printVector(&vec);cout<<endl;
	fflush(stdout);
	vec.reserve(N + 2 * K);

	cout<<"Sorter1:"<<endl;
	Sorter1 sorter1(&vec, K, N);
	sorter1.gantrySort();
	sorter1.printElements();
	cout<<"Liczba ruchów suwnicy: "<<sorter1.getGantryMovesCount()<<endl<<endl;


	cout<<"Sorter2:"<<endl;
	Sorter2 sorter2 = Sorter2(&vec, K, N);
	sorter2.gantrySort();
	sorter2.printElements();
	cout<<"Liczba ruchów suwnicy: "<<sorter2.getGantryMovesCount()<<endl<<endl;
#endif

	cout<<"Koniec..."<<endl;

	return 0;
}
