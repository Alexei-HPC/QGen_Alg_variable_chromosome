#pragma once

#include <vector>

using namespace std;

struct bus
{
	int capacity;	//Вместительность
	double lambda;	//Коэффициент затрат на прохождение ребра маршрута
	int bus_label;	//Номер автобуса
};

class Rout
{

public:
	int max_interval;
	int trip_numbers; //Число рейсов == числу автобусов
    int rout_label; //номер маршрута
	pair <int, int> gap; //Время работы маршрута
	vector <unsigned long> stops; //Номера остановк маршрута
	vector <bus> buses;//Автобусы capacity lambda
	Rout(void);
	~Rout(void);
};

