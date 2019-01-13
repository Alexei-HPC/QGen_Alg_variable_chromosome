#pragma once

#include <vector>
#include "tinyxml.h"
#include <utility> 
using namespace std;

class PassengersDistribution
{

public:
	struct intention 
	{
		int passangers_direction;
		int passangers_number;
	};

	struct people
	{ 
		vector<intention> intentions;
		int total_passangers;
		int comed_passangers;
		int remain_passangers;
	};

	struct bus_stop
	{
		unsigned long label_of_bus_stop;
		vector <pair<int, people>> intentions_by_time;
	};

	vector <bus_stop> array_of_stops;


	PassengersDistribution(void);
	PassengersDistribution(string path);
	int Distribution_load(string path);
	int GetPassangers(unsigned long label_of_bus_stop, int passangers_number, int time);
	int PassNumber(unsigned long label_of_bus_stop, int time);
	int PrintDisrtibution();
	int PrintBusStop(unsigned long bus_stop_label);
	//vector <pair<int, people>> Intentions();
	~PassengersDistribution(void);
};

