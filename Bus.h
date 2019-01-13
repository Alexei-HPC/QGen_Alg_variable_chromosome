#pragma once

#include <vector>
#include <iostream>
#include "EdgeCost.h"
#include "MinimalDriveTime.h"
#include "Routes.h"
#include "Individual.h"
#include "Events.h"
#include "Passanger.h"

using namespace std;

class Bus
{
	struct edge
	{
		unsigned long a;	//Начало ребра
		unsigned long b;	//Конец ребра

		int departure_time;	//Время начала пути по данному ребру
		int time;	//Время на преодоление ребра
		int cost;	//Цена преодоления ребра
	};

public:
	int my_rout_max_interval;	//Максимальный разрешенный на маршруте интервал между автобусами
	int bus_label;	//Номер автобуса
	int capacity;	//Вместительность автобуса
	int free_places;//Количество свободных мест
	int rout_label;	//Номер маршрута

	int start_time;	//Время выхода автобуса в рейс
	int time_in_way;//Время автобуса в пути

	double lambda;	//Коэффициент затрат на преодоление ребер автобусом

	//vector < pair <unsigned long, int> > intentions;	//Вектор количества пассажиров(first) едущих по направлению(second)
	vector <Passanger> passangers;
	vector < unsigned long > bus_stops;	//остановки автобуса, упорядоченные 
	vector < edge > ed_time_cost;	

	double passangers_wt_fines;
	double remain_passangers;	//Штрафы
	double cost;	//Цены

	Bus(void);
	Bus(int rout_label, int bus_label, int capacity, double lambda);
	int PrintBusInfo();	//Вывод информации об автобусе
	int GetBusEvents(Events &events_vector);
	int TimeInit(int time);	//инициализация времени выезда автобуса в рейс
	int RoutInit(vector < unsigned long > stops);	//Инициализация остановок
	int MinimalTimesCalculation(MinimalDriveTime min_driv_time);	//расчет времени прохождения автобуса по ребрам
	int MinimalCostsCalculation(EdgeCost min_base_costs);	//расчет стоимостей прохода автобуса по ребрам
	~Bus(void);
};

