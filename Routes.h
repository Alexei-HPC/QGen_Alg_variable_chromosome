#pragma once

#include <vector>
#include "tinyxml.h"
#include "Rout.h"
using namespace std;

class Routes
{
public:
	vector<Rout> routes; //����� ���������

	Routes(void);
	Routes(string path);
	int PrintRoutes();	//����� ���������� � ���������
	int RoutesLoad(string path);	//�������� ���������
	int ChromosomeSize();	//����������� ����� ���������
	int GetGaps(vector< pair <int, pair<int, int>>> &routs_gaps); //���������� ������� ������������ ������ ���������
	~Routes(void);
};

