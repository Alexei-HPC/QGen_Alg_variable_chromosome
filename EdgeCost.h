#pragma once

#include <vector>
#include "tinyxml.h"
#include <algorithm> 

using namespace std;

class EdgeCost
{
	struct edge//—труктура ребра
	{
		unsigned long a;
		unsigned long b;
		vector <pair <int, double>> time_cost;//пары врем€ - стоимость
	};

private:
	vector <edge> cost_graph;
	double minimal_cost;

public:

	EdgeCost(void);
	EdgeCost(string path, double min_cost);
	int EdgeCostInit(string path);//—читывание информации о стоимости проезда в соответствующие промежутки времени
	double GetEdgeCost(unsigned long a, unsigned long b, int time);//¬озвражает стоимость проезда по ребру в соответствующее врем€. ≈сли нет соответствующего времени вернет -1
	int EdgeCostsPrint();//ѕечать данных о стоимости
	~EdgeCost(void);
};

