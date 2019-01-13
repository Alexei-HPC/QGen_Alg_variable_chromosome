#pragma once

#include <vector>
#include "tinyxml.h"
#include <algorithm> 

using namespace std;

class EdgeCost
{
	struct edge//��������� �����
	{
		unsigned long a;
		unsigned long b;
		vector <pair <int, double>> time_cost;//���� ����� - ���������
	};

private:
	vector <edge> cost_graph;
	double minimal_cost;

public:

	EdgeCost(void);
	EdgeCost(string path, double min_cost);
	int EdgeCostInit(string path);//���������� ���������� � ��������� ������� � ��������������� ���������� �������
	double GetEdgeCost(unsigned long a, unsigned long b, int time);//���������� ��������� ������� �� ����� � ��������������� �����. ���� ��� ���������������� ������� ������ -1
	int EdgeCostsPrint();//������ ������ � ���������
	~EdgeCost(void);
};

