#pragma once
#include <vector>
#include "Individual.h"
using namespace std;

class Population
{
public:
	vector <Individual> individuals; //����� ������
	int chromosome_size; //����� ���������
	int population_size;//���������� ������
	vector< pair <int, pair<int, int>>> routs_gaps; //������� �������� ����� �� ������ �� �������� ���������

	Population(void);
	Population(int pop_size, int ch_size, vector< pair <int, pair<int, int>>> routs_gaps);
	int Mutation(double probability, int mut_number);//�������
	int Crossover(double probability);//�����������
	int PrintPopulation(void);//����� ����� ������� �������� ���������
	int Selection(int truncation);//����� � ���������
	~Population(void);
};

