#pragma once

#include "Bus.h"
#include <vector>
#include <algorithm>
using namespace std;

class Buses
{
public:
	vector <Bus> buses;
	Buses(void);
	int BusesInit(Routes routes, Individual individ);
	int TimesAndCostsCalc(MinimalDriveTime mindrivetime, EdgeCost minbcost);	//расчет времен и стоимостей прохождения автобусами их маршрутов
	double BusesAllCosts();	//Расчет стоимостей прохода автобусов по их ребрам
	double BusesAllRemainPassangers();	//Расчет времени прохождения автобусов по их ребрам
	int PrintBusList();	//Вывод информации об автобусах
	int EventsCreate(Events &ev);	//Создание списка событий прибытия автобусов на остановку
	~Buses(void);
};

