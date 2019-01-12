#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Event
{
	
public:
	static bool Compair(Event a, Event b);
	int bus_label;	//Номер автобуса
	int rout_label;	//Номер маршрута
	unsigned long bus_stop_label;	//Номер остановки
	int rout_max_interval;	//максимальный интервал между автобусами на маршруте текущего автобуса 
	int time;	//Время прибытия автобуса на остановку

	Event(void);
	Event(int bus_label, int rout_label, unsigned long bus_stop_label, int time);
	int Print(); //Вывод информации о событии прибытия автобуса на остановку
	~Event(void);	
};

