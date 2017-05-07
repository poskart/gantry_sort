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
	int N = 25;
	vector<int> vec;
	//vec = evenlyGenerator(K, N);
	vec = {3, 3, 1, 2, 2, 4, 1, 2, 3, 1, 4, 3, 2, 2, 3, 4, 3, 1, 2, 3, 4, 2, 3, 3, 4};


	printVector(&vec);
	fflush(stdout);
//	vec.reserve(N + 2 * K);
//
//	cout<<"Sorter1:"<<endl;
//	Sorter1 sorter1(&vec, K, N);
//	sorter1.gantrySort();
//	sorter1.printElements();
//	cout<<"Sorter1 end..."<<endl;
//
//
//	cout<<"Wektor testowy: "<<endl;
//	K = 5;
//	vector<int> test = {4, 2, 1, 6, 1, 3, 5};
//	test.reserve(N + 2 * K);
//	SystematicFinder finder = SystematicFinder(&test, K, 0);
//	finder.sortLastBatch();
//	printVector(&test);

	Sorter2 sorter2 = Sorter2(&vec, K, N);
	sorter2.gantrySort();
	sorter2.printElements();

	cout<<"\nLiczba ruchów suwnicy: "<<Gantry::gantryMoves<<endl;
	cout<<"Koniec..."<<endl;

	return 0;
}
