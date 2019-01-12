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
	int * chromosome; //���������
	int chromosome_size; //����� ���������
	double fitness_value; //�������� ����������������� �����
	vector< pair <int, pair<int, int>>> routs_gaps; //������� �������� ����� �� ������ �� �������� ���������

	Individual(void);
	Individual(const Individual &tmp);
	Individual(int chromosome_size, vector< pair <int, pair<int, int>>> gaps);
	int PrintIndivid(void);//����� ����� ��������
	int Mutation(double probability, int mut_number);//�������
	~Individual(void);
};

