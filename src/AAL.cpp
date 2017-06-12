//============================================================================
// Name        : AAL.cpp
// Author      : Piotr Poskart
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <functional>
#include <queue>
#include "SystematicFinder.h"
#include "Sorter1.h"
#include "Sorter2.h"
#include "Sorter3.h"

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

int main(int argc, char ** argv)
{
	int K = 15;
	int N = 10000;
	int outerLoopVariable = 'k';
	char mode[4];
	int * kTab, *nTab;
	int parametersTable[6] = {15, 15, 61, 10000, 1, 10001};
	vector<int> vec;

	if(argc < 2)
	{
		printf("%s AAL <commands>\n", argv[0]);
		printf(
			"Availible commands: \n"
			"-m1 input_filename\n"
			"-m2 -k:firstk:stepk:lastk -n:firstn:stepn:lastn\n"
			"-m3 -kvalue x x x ... x (x - elements)\n"
		);
		return 0;
	}
	strcpy(mode, argv[1]);

	if(strcmp(mode, "-m1") == 0 && argc == 3)
	{
		std::fstream file(argv[2], std::ios_base::in);
		if(file.is_open())
		{
			for(int i = 0; i < 6; i++)
				file >> parametersTable[i];
		}
		kTab = parametersTable;
		nTab = &parametersTable[3];
	}
	else if(strcmp(mode, "-m2") == 0 && argc == 4)
	{
		char * pch;
		pch = strtok (argv[2],"-:");
		if(*pch == 'n')
		{
			outerLoopVariable = 'n';
			kTab = &parametersTable[3];
			nTab = parametersTable;
		}
		else
		{
			kTab = parametersTable;
			nTab = &parametersTable[3];
		}
		for(int i = 0; i < 3; i++)
		{
			pch = strtok (NULL, "-:");
			if(pch != NULL)
				parametersTable[i] = atoi(pch);
		}
		pch = strtok (argv[3],"-:");
		for(int i = 3; i < 6; i++)
		{
			pch = strtok (NULL, "-:");
			if(pch != NULL)
				parametersTable[i] = atoi(pch);
		}
	}
	else if(strcmp(mode, "-m3") == 0)
	{
		char * pch;
		pch = strtok (argv[2],"-k");
		// save k
		parametersTable[0] = atoi(pch);
		parametersTable[3] = argc-3;
		for(int i = 3; i < argc; i++)
		{
			vec.push_back(atoi(argv[i]));
		}
	}
	else if(strcmp(mode, "-m4") == 0)
	{
		char * pch;
		pch = strtok (argv[2],"-k");
		// save k
		parametersTable[0] = atoi(pch);
		parametersTable[3] = argc-3;
		for(int i = 3; i < argc; i++)
		{
			vec.push_back(atoi(argv[i]));
		}
	}

	/**********************************   TEST EXECUTION   **********************************/

	/**
	 * Mode 2, Mode 1
	 */
	if(strcmp(mode, "-m2") == 0 || strcmp(mode, "-m1")== 0)
	{
		if(outerLoopVariable == 'k')
			for(K = kTab[0]; K < kTab[2]; K+=kTab[1])
			{
				cout<<"\n\t\tK = "<<K<<endl;
				cout<<"N\tSorter1 time[us]:\tSorter2 time[us]:"<<endl;
				for(N = nTab[0]; N < nTab[2]; N+=nTab[1])
				{
					vec = generator(K, N);
					vec.reserve(N + 2 * K);

					Sorter1 sorter1(&vec, K, N);
					sorter1.gantrySort();
					Sorter2 sorter2 = Sorter2(&vec, K, N);
					sorter2.gantrySort();

					cout<<N<<"\t"<<sorter1.getSortingTime()<<"\t\t\t"<<sorter2.getSortingTime()<<endl;
				}
			}
		else
			for(N = nTab[0]; N < nTab[2]; N+=nTab[1])
			{
				cout<<"\n\t\tN = "<<N<<endl;
				cout<<"K\tSorter1 time[us]:\tSorter2 time[us]:"<<endl;
				for(K = kTab[0]; K < kTab[2]; K+=kTab[1])
				{
					vec = generator(K, N);
					vec.reserve(N + 2 * K);

					Sorter1 sorter1(&vec, K, N);
					sorter1.gantrySort();
					Sorter2 sorter2 = Sorter2(&vec, K, N);
					sorter2.gantrySort();

					cout<<K<<"\t"<<sorter1.getSortingTime()<<"\t\t\t"<<sorter2.getSortingTime()<<endl;
				}
			}
	}
	/**
	 * Mode 3
	 */
	else if(strcmp(mode, "-m3") == 0)
	{
		K = parametersTable[0];
		N = parametersTable[3];
		cout<<"N = "<<N<<", K = "<<K<<endl;
		printHistogram(&vec, K);

		printVector(&vec);cout<<endl;
		fflush(stdout);
		vec.reserve(N + 2 * K);

		cout<<"Sorter1 (dokładanie do lewej):"<<endl;
		Sorter1 sorter1(&vec, K, N);
		sorter1.gantrySort();
		sorter1.printElements();
		cout<<"Liczba ruchów suwnicy: "<<sorter1.getGantryMovesCount()<<", Czas [us]: "<<sorter1.getSortingTime()<<endl<<endl;

		cout<<"Sorter2 (układanie po prawej stronie):"<<endl;
		Sorter2 sorter2 = Sorter2(&vec, K, N);
		sorter2.gantrySort();
		sorter2.printElements();
		cout<<"Liczba ruchów suwnicy: "<<sorter2.getGantryMovesCount()<<", Czas [us]: "<<sorter2.getSortingTime()<<endl<<endl;

		cout<<"Sorter3 (dokładanie do lewej z wyszukiwaniem max podciagow):"<<endl;
		Sorter3 sorter3 = Sorter3(&vec, K, N);
		sorter3.gantrySort();
		sorter3.printElements();
		cout<<"Liczba ruchów suwnicy: "<<sorter3.getGantryMovesCount()<<", Czas [us]: "<<sorter3.getSortingTime()<<endl<<endl;
	}
	else if(strcmp(mode, "-m4") == 0)
	{
		int K = 4, N = 39;
		vec = {1, 3, 2, 2, 3, 3, 3, 3, 2, 1, 1, 2, 3, 4, 1, 4, 1, 1, 4, 1, 3, 3, 2, 1, 4, 1, 3, 1, 2, 3, 1, 2, 2, 2, 3, 4, 1, 1, 2};
		//vec = generator(K, N);
		printVector(&vec);
		cout<<"Batches count: "<<getBatchesCount(&vec, K)<<endl;
		Sorter2 sorter(&vec, K, N);
		sorter.gantrySort();
		sorter.printElements();
		cout<<"Gantry moves: "<<sorter.getGantryMovesCount()<<endl;
	}

	return 0;
}
