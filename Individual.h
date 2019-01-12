#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <conio.h>
#include <Windows.h>
#include <iomanip>

using namespace std;

class Individual
{
private:

public:
	int * chromosome; //Хромосома
	int chromosome_size; //Длина хромосомы
	double fitness_value; //Значение приспособленности особи
	vector< pair <int, pair<int, int>>> routs_gaps; //границы значений генов на каждом из участков хромосомы

	Individual(void);
	Individual(const Individual &tmp);
	Individual(int chromosome_size, vector< pair <int, pair<int, int>>> gaps);
	int PrintIndivid(void);//Вывод генов индивида
	int Mutation(double probability, int mut_number);//Мутация
	~Individual(void);
};

