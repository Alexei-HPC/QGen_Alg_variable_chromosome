#pragma once

#include <vector>
#include "tinyxml.h"
#include "Rout.h"
using namespace std;

class Routes
{
public:
	vector<Rout> routes; //Набор маршрутов

	Routes(void);
	Routes(string path);
	int PrintRoutes();	//Вывод информации о маршрутах
	int RoutesLoad(string path);	//загрузка маршрутов
	int ChromosomeSize();	//определение длины хромосомы
	int GetGaps(vector< pair <int, pair<int, int>>> &routs_gaps); //заполнение вектора промежутками работы маршрутов
	~Routes(void);
};

