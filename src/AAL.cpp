//============================================================================
// Name        : AAL.cpp
// Author      : Piotr Poskart
// Version     :
// Copyright   : Your copyright notice
// Description :
// Tryby wykonania programu:
// -m1	<nazwa pliku> - program wczytuje z pliku parametry do testów w kolejności odpowiednio:
//		wartość początkowa k, krok k, wartość końcowa k, wartość początkowa n, krok n, wartość końcowa n.
//		Następnie program wykonuje testy czasu wykonania dla zmieniających się wartości k i n.
// -m2 -k:firstk:stepk:lastk -n:firstn:stepn:lastn – uruchamia testy dla k w zakresie <firstk, lastk> z krokiem  stepk,
//		oraz dla n w zakresie <firstn, lastn> z krokiem  stepn. Jeśli sekwencja -k:firstk:stepk:lastk zostanie podana
//		jako pierwsza, wyniki drukowane są dla stałego k  i zmieniającego się n. Natomiast jeśli jako pierwsza zostanie
//		podana sekwencja -n:firstn:stepn:lastn wyniki drukowane są na standardowe wyjście dla stałego n i zmiennych wartości k.
// -m3 -kwartosc x x x x x x x x…..x – wykonywany jest pojedynczy test algorytmów sortujących dla danego k o wartości wartosc
//		dla podanej sekwencji elementów x x x x … x. Na standardowe wyjście drukowany jest histogram podanego ciągu, ilość zestawów,
//		wynikowe ciągi po sortowaniu poszczególnymi algorytmami, czas sortowania oraz liczba ruchów suwnicy.
// -m4 -k:firstk:stepk:lastk -n:firstn:stepn:lastn -program wykonuje test złożoności dla podanych, ustalonych k
//		(z przedziału <firstk, lastk> z krokiem stepk) oraz n zmieniającego się od  firstn do lastn z krokiem  stepn.
//		Po wykonaniu testu drukowane są informacje o obliczonym współczynniku q(n).
// -m5 -kwartosc x x x x x x x x…..x – podobnie jak -m3, jednak po każdym kroku głównej pętli każdego z algorytmów wyświetlany jest aktualny wektor.
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
#include <iomanip>
#include "SystematicFinder.h"
#include "Sorter1.h"
#include "Sorter2.h"
#include "Sorter3.h"

using namespace std;
using std::setw;

//#define CHECK 1

vector<int> generator(int k, int n)
{
	srand(time(NULL));
	vector<int> testTable;
	for (int i = 0; i < n; i++)
	{
		testTable.push_back(rand() % k + 1);
	}
	return testTable;
}

vector<int> evenGenerator(int k, int n)
{
	vector<int> vec;
	int setCount = n / k;
	for (int i = 0; i < setCount; i++)
	{
		for (int j = 1; j <= k; j++)
			vec.push_back(j);
	}
	srand(time(NULL));
	// Fill remaining places
	for (int i = 0; i < (n - setCount * k); i++)
	{
		vec.push_back(rand() % k + 1);
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
	int parametersTable[6] =
	{ 15, 15, 61, 10000, 1, 10001 };
	//k, kstep, klimit    n, nstep, nlimit - values in parametersTable above
	vector<int> vec;

	if (argc < 2)
	{
		printf("%s AAL <commands>\n", argv[0]);
		printf("Availible commands: \n"
				"-m1 input_filename\n"
				"-m2 -k:firstk:stepk:lastk -n:firstn:stepn:lastn\n"
				"-m3 -kvalue x x x ... x (x - elements)\n");
		return 0;
	}
	strcpy(mode, argv[1]);

	if (strcmp(mode, "-m1") == 0 && argc == 3)
	{
		std::fstream file(argv[2], std::ios_base::in);
		if (file.is_open())
		{
			for (int i = 0; i < 6; i++)
				file >> parametersTable[i];
		}
		kTab = parametersTable;
		nTab = &parametersTable[3];
	}
	else if ((strcmp(mode, "-m2") == 0 || strcmp(mode, "-m4") == 0)
			&& argc == 4)
	{
		char * pch;
		pch = strtok(argv[2], "-:");
		if (*pch == 'n')
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
		for (int i = 0; i < 3; i++)
		{
			pch = strtok(NULL, "-:");
			if (pch != NULL)
				parametersTable[i] = atoi(pch);
		}
		pch = strtok(argv[3], "-:");
		for (int i = 3; i < 6; i++)
		{
			pch = strtok(NULL, "-:");
			if (pch != NULL)
				parametersTable[i] = atoi(pch);
		}
	}
	else if (strcmp(mode, "-m3") == 0 || strcmp(mode, "-m5") == 0)
	{
		char * pch;
		pch = strtok(argv[2], "-k");
		// save k
		parametersTable[0] = atoi(pch);
		parametersTable[3] = argc - 3;
		for (int i = 3; i < argc; i++)
		{
			vec.push_back(atoi(argv[i]));
		}
	}
	else if (strcmp(mode, "-m4") == 0)
	{

	}

	/**********************************   TEST EXECUTION   **********************************/

	/**
	 * Mode 2, Mode 1
	 */
	if (strcmp(mode, "-m2") == 0 || strcmp(mode, "-m1") == 0)
	{
		if (outerLoopVariable == 'k')
			for (K = kTab[0]; K < kTab[2]; K += kTab[1])
			{
				cout << "\n\t\tK = " << K << endl;
				cout << setw(10)<<left<<"N"
						<<setw(14)<<"S1 time[us]:"<<setw(10)<<"S1 moves:"
						<<setw(14)<<"S2 time[us]:"<<setw(10)<<"S2 moves:"
						<<setw(14)<<"S3 time[us]:"<<setw(10)<<"S3 moves:"<<endl;
				for (N = nTab[0]; N < nTab[2]; N += nTab[1])
				{
					vec = generator(K, N);
					vec.reserve(N + 2 * K);

					Sorter1 sorter1(&vec, K, N);
					sorter1.gantrySort();
					Sorter2 sorter2 = Sorter2(&vec, K, N);
					sorter2.gantrySort();
					Sorter3 sorter3 = Sorter3(&vec, K, N);
					sorter3.gantrySort();

					cout <<setw(10) <<N
							<< setw(14) << sorter1.getSortingTime()
							<< setw(10) << sorter1.getGantryMovesCount()
							<< setw(14) << sorter2.getSortingTime()
							<< setw(10) << sorter2.getGantryMovesCount()
							<< setw(14) << sorter3.getSortingTime()
							<< setw(10) << sorter3.getGantryMovesCount()<<endl;
				}
			}
		else
			for (N = nTab[0]; N < nTab[2]; N += nTab[1])
			{
				cout << "\n\t\tN = " << N << endl;
				cout << setw(10)<<left<<"K"
						<<setw(14)<<"S1 time[us]:"<<setw(10)<<"S1 moves:"
						<<setw(14)<<"S2 time[us]:"<<setw(10)<<"S2 moves:"
						<<setw(14)<<"S3 time[us]:"<<setw(10)<<"S3 moves:"<<endl;
				for (K = kTab[0]; K < kTab[2]; K += kTab[1])
				{
					vec = generator(K, N);
					vec.reserve(N + 2 * K);

					Sorter1 sorter1(&vec, K, N);
					sorter1.gantrySort();
					Sorter2 sorter2 = Sorter2(&vec, K, N);
					sorter2.gantrySort();
					Sorter3 sorter3 = Sorter3(&vec, K, N);
					sorter3.gantrySort();

					cout <<setw(10) <<K
						<< setw(14) << sorter1.getSortingTime()
						<< setw(10) << sorter1.getGantryMovesCount()
						<< setw(14) << sorter2.getSortingTime()
						<< setw(10) << sorter2.getGantryMovesCount()
						<< setw(14) << sorter3.getSortingTime()
						<< setw(10) << sorter3.getGantryMovesCount()<<endl;
				}
			}
	}
	/**
	 * Mode 3
	 */
	else if (strcmp(mode, "-m3") == 0)
	{
		K = parametersTable[0];
		N = parametersTable[3];
		cout << "N = " << N << ", K = " << K << endl;
		printHistogram(&vec, K);

		printVector(&vec);
		cout << endl;
		fflush(stdout);
		vec.reserve(N + 2 * K);

		cout << "Sorter1 (dokładanie do lewej):" << endl;
		Sorter1 sorter1(&vec, K, N);
		sorter1.gantrySort();
		sorter1.printElements();
		cout << "Liczba ruchów suwnicy: " << sorter1.getGantryMovesCount()
				<< ", Czas [us]: " << sorter1.getSortingTime() << endl << endl;

		cout << "Sorter2 (układanie po prawej stronie):" << endl;
		Sorter2 sorter2 = Sorter2(&vec, K, N);
		sorter2.gantrySort();
		sorter2.printElements();
		cout << "Liczba ruchów suwnicy: " << sorter2.getGantryMovesCount()
				<< ", Czas [us]: " << sorter2.getSortingTime() << endl << endl;

		cout << "Sorter3 (dokładanie do lewej z wyszukiwaniem max podciagow):"
				<< endl;
		Sorter3 sorter3 = Sorter3(&vec, K, N);
		sorter3.gantrySort();
		sorter3.printElements();
		cout << "Liczba ruchów suwnicy: " << sorter3.getGantryMovesCount()
				<< ", Czas [us]: " << sorter3.getSortingTime() << endl << endl;
	}
	else if (strcmp(mode, "-m4") == 0)
	{
		if (outerLoopVariable == 'k')
			for (K = kTab[0]; K < kTab[2]; K += kTab[1])
			{
				vector<long> t1, t2, t3;
				vector<int> gm1, gm2, gm3;
				long double c1, c2, c3;
				int i = 0;
				for (N = nTab[2]; N >= nTab[0]; N -= nTab[1])
				{
					vec = generator(K, N);
					vec.reserve(N + 2 * K);

					Sorter1 sorter1(&vec, K, N);
					sorter1.gantrySort();
					t1.push_back(sorter1.getSortingTime());
					gm1.push_back(sorter1.getGantryMovesCount());
					Sorter2 sorter2 = Sorter2(&vec, K, N);
					sorter2.gantrySort();
					t2.push_back(sorter2.getSortingTime());
					gm2.push_back(sorter2.getGantryMovesCount());
					Sorter3 sorter3 = Sorter3(&vec, K, N);
					sorter3.gantrySort();
					t3.push_back(sorter3.getSortingTime());
					gm3.push_back(sorter3.getGantryMovesCount());
					i++;
				}
				N = nTab[2] - (i/2)*nTab[1];
				c1 = t1.at(i / 2)
						/ (long double) (Sorter1::complexity((long int) K,
								(long int) N));
				c2 = t2.at(i / 2)
						/ (long double) (Sorter2::complexity((long int) K,
								(long int) N));
				c3 = t3.at(i / 2)
						/ (long double) (Sorter3::complexity((long int) K,
								(long int) N));
				cout << "\n\t\tK = " << K << endl;
				cout << setw(10)<<"N"<<setw(19)<<"Sorter1 time[us]:"<<setw(10)
						<<"q(n):"<<setw(19)<<"Sorter2 time[us]:"<<setw(10)<<"q(n):"
						<<setw(19)<<"Sorter3 time[us]:"<<setw(10)<<"q(n):"
						<< endl;
				for (int index = 0; index < i; index++)
				{
					cout << setw(10) <<nTab[2] - index * nTab[1];
					cout << setw(19) << t1.at(index);
					cout << setw(10)
							<< t1.at(index)
									/ ((long double) c1
											* Sorter1::complexity((long int) K,
													(long int) (nTab[2]
															- index * nTab[1])));
					cout << setw(19) << t2.at(index);
					cout << setw(10)
							<< t2.at(index)
									/ ((long double) c2
											* Sorter2::complexity((long int) K,
													(long int) (nTab[2]
															- index * nTab[1])));
					cout << setw(19) << t3.at(index);
					cout << setw(10)
							<< t3.at(index)
									/ ((long double) c3
											* Sorter3::complexity((long int) K,
													(long int) (nTab[2]
															- index * nTab[1])))
							<< endl;
				}
			}
	}
	else if (strcmp(mode, "-m5") == 0)
	{
		K = parametersTable[0];
		N = parametersTable[3];
		cout << "N = " << N << ", K = " << K << endl;
		printHistogram(&vec, K);

		printVector(&vec);
		cout << endl;
		fflush(stdout);
		vec.reserve(N + 2 * K);

		cout << "Sorter1 (dokładanie do lewej):" << endl;
		Sorter1 sorter1(&vec, K, N);
		sorter1.gantrySortWatch();
		sorter1.printElements();
		cout << "Liczba ruchów suwnicy: " << sorter1.getGantryMovesCount()
				<< ", Czas [us]: " << sorter1.getSortingTime() << endl << endl;

		cout << "Sorter2 (układanie po prawej stronie):" << endl;
		Sorter2 sorter2 = Sorter2(&vec, K, N);
		sorter2.gantrySortWatch();
		sorter2.printElements();
		cout << "Liczba ruchów suwnicy: " << sorter2.getGantryMovesCount()
				<< ", Czas [us]: " << sorter2.getSortingTime() << endl << endl;

		cout << "Sorter3 (dokładanie do lewej z wyszukiwaniem max podciagow):"
				<< endl;
		Sorter3 sorter3 = Sorter3(&vec, K, N);
		sorter3.gantrySortWatch();
		sorter3.printElements();
		cout << "Liczba ruchów suwnicy: " << sorter3.getGantryMovesCount()
				<< ", Czas [us]: " << sorter3.getSortingTime() << endl << endl;

	}
	cout << "Koniec" << endl;

	return 0;
}
