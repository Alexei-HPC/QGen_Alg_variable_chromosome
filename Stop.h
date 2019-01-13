#pragma once
#include <vector>
#include "tinyxml.h"
#include <algorithm>
#include <fstream>
#include "Passanger.h"
#include "Bus.h"
#include "Parameters_.h"

using namespace std;

struct intentions
{
	unsigned long direction;	//Направление
	int passangers_number;	//Число пассажиров
};	

struct timedensity
{
	int time;	//Время
	int density_val;	//Число пришедших пассажиров
};

struct dir_td
{
	unsigned long dir;	//Направление
	vector <timedensity> vtimedens; //Приходы пассажиров во времени
};

class Stop
{
public:
	unsigned long bus_stop_label; //Номер остановки
	int total_passangers; //Общее число пассажиров на остановке
	//vector<intentions> remain_passangers; //Пассажиры стоящие на остановке + их направления
	vector< Passanger > passangers;//Пассажиры, стоящие на остановке, и вся информация о них
	int time_of_last_rout; //Время, когда приходил прошлый автобус
	vector<dir_td> vdensity; //Пары < направление вектов <время_плотность>>

	int Recalc(int current_time); //Переращет количества пассажиров на текущей остановке
	int landLoadPassangers(/*vector <pair<unsigned long, int>> &bus_intentions*/ vector<Passanger> &bus_pussangers, int &free_places, int current_time, vector <unsigned long> bus_stops, Parameters_ parameters, Bus &bus);	//Высадка и посадка пассажиров на остановке
	int RemainPassangers(); //Возвращает количество пассажиров пришедших до конца дня после последнего автобуса
	int Print();	//Вывод информации о остановке
	Stop(void);
	Stop(unsigned long pbus_stop_label);
	~Stop(void);
};

