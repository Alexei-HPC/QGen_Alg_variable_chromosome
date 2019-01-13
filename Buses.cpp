#include "Buses.h"


Buses::Buses(void)
{
}

int Buses::TimesAndCostsCalc(MinimalDriveTime mindrivetime, EdgeCost minbcost)
{
	for(int i = 0; i < buses.size(); i++)
	{
		buses[i].MinimalTimesCalculation(mindrivetime);
		//buses[i].MinimalCostsCalculation(minbcost);
	}
	return 0;
}

double  Buses::BusesAllCosts()
{
	double costs = 0;

	for(unsigned long i = 0; i < buses.size(); i++)
	{
		costs += buses[i].cost;
	}

	return costs;
}

double Buses::BusesAllRemainPassangers()
{
	double remain_passangers = 0;

	for(unsigned long i = 0; i < buses.size(); i++)
	{
		remain_passangers += buses[i].remain_passangers;
	}

	return remain_passangers;
}

int Buses::PrintBusList()
{
	for(unsigned long i = 0; i < buses.size(); i++)
	{
		buses[i].PrintBusInfo();
	}
	return 0;
}

int Buses::EventsCreate(Events &events_vec)
{
	//int bus_label;
	//int bus_stop_label;
	//int rout_label;

	for(int i = 0; i < buses.size(); i++)
	{
		buses[i].GetBusEvents(events_vec);
	}

	sort(events_vec.events_vector.begin(), events_vec.events_vector.end(), Event::Compair);

	return 0;
}



int Buses::BusesInit(Routes routes, Individual individ)
{
	int chromosome_size = individ.chromosome_size;
	int buses_number = 0;

	for(int i = 0; i < routes.routes.size(); i++)
	{
			buses_number += routes.routes[i].buses.size();
	}

	if(buses_number != chromosome_size)	//Проверка. Число автобусов должно равняться длине гена
	{
		cout<<"Error! Dimentions are not equal!"<<endl;
		cout<<"Chromosome size: "<<chromosome_size<<endl;
		cout<<"Buses number: "<<buses_number<<endl;
		return 1;
	}

	int counter = 0;
	for(int i = 0; i < routes.routes.size(); i++)
	{
		for(int j = 0; j < routes.routes[i].buses.size(); j++)
		{
			Bus bus(routes.routes[i].rout_label, routes.routes[i].buses[j].bus_label, routes.routes[i].buses[j].capacity, routes.routes[i].buses[j].lambda);
			bus.TimeInit(individ.chromosome[counter]);
			bus.RoutInit(routes.routes[i].stops);
			bus.my_rout_max_interval = routes.routes[i].max_interval;
			buses.push_back(bus);
			counter++;
		}
	}

	return 0;
}


Buses::~Buses(void)
{
}
