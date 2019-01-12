#pragma once
#include <vector>
#include "Individual.h"
using namespace std;

class Population
{
public:
	vector <Individual> individuals; //Набор особей
	int chromosome_size; //Длина хромосомы
	int population_size;//Количество особей
	vector< pair <int, pair<int, int>>> routs_gaps; //границы значений генов на каждом из участков хромосомы

	Population(void);
	Population(int pop_size, int ch_size, vector< pair <int, pair<int, int>>> routs_gaps);
	int Mutation(double probability, int mut_number);//Мутация
	int Crossover(double probability);//Скрещивание
	int PrintPopulation(void);//Вывод генов каждого индивида популяции
	int Selection(int truncation);//Отбор с усечением
	~Population(void);
};

