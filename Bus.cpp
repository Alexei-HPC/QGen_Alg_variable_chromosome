#include "Bus.h"


Bus::Bus(void)
{
	free_places = capacity;
	remain_passangers = 0;
	cost = 0;
	start_time = 0;
	time_in_way = 0;
	passangers_wt_fines = 0;
}

Bus::Bus(int prout_label, int pbus_label, int pcapacity, double plambda)
{
	rout_label = prout_label;
	bus_label = pbus_label;
	capacity = pcapacity;
	lambda = plambda;

	free_places = capacity;
	remain_passangers = 0;
	cost = 0;
	start_time = 0;
	time_in_way = 0;
	passangers_wt_fines = 0;
}

int Bus::GetBusEvents(Events &events_vector)	//Создаем вектор событий. Событие - прибытие автобуса на остановку
{
	for(int i = 0; i < ed_time_cost.size(); i++)
	{
		Event ev;
		ev.bus_label = bus_label;
		ev.bus_stop_label = ed_time_cost[i].a;
		ev.rout_label = rout_label;
		ev.time = ed_time_cost[i].departure_time;
		events_vector.events_vector.push_back(ev);
	}
	return 0;
}

int Bus::TimeInit(int ptime)
{
	start_time = ptime;
	return 0;
}

int Bus::PrintBusInfo()	//Вывод информации об автобусе
{
	cout<<endl;
	cout<<"Bus information: "<<endl;
	cout<<"Rout label: "<<rout_label<<endl;
	cout<<"Bus label: "<<bus_label<<endl;
	cout<<"Bus capasity: "<<capacity<<endl;
	cout<<"Free places number: "<<free_places<<endl;
	cout<<"Bus rout: "<<endl;
	for(int i = 0; i < bus_stops.size(); i++)
	{
		cout<< bus_stops[i]<<" ";
	}
	cout<<endl;

	cout<<"Bus moves: "<<endl;
	for(int i = 0; i < ed_time_cost.size(); i++)
	{
		cout << ed_time_cost[i].a 
			<< " " 
			<< ed_time_cost[i].b 
			<< " departure time: " 
			<< ed_time_cost[i].departure_time
			<< " time in way: "
			<< ed_time_cost[i].time
			<<endl;
	}
	cout<<endl;
	cout<<"Time to the way: "<<time_in_way<<endl;

	return 0;
}

int Bus::RoutInit(vector < unsigned long > stops)	//Инициализация маршрута
{
	bus_stops.clear();
	bus_stops.resize(stops.size());

	for(int i = 0; i < stops.size(); i++)
	{
		bus_stops[i] = stops[i];
	}

	ed_time_cost.clear();
	for(int i = 1; i < stops.size(); i++)
	{
		edge e;
		e.a = stops[i - 1];
		e.b = stops[i];
		e.cost = -1;
		e.time = -1;
		e.departure_time = 0;
		ed_time_cost.push_back(e);
	}

	return 0;
}

int Bus::MinimalTimesCalculation(MinimalDriveTime min_driv_time)	//расчет времени прохода по ребрам маршрута
{
	if(bus_stops.size() <= 0)	//Если нет остановок
	{
		cout<<"Error! Bus stops not found."<<endl;
		return -1;
	}

	int common_time = start_time;
	if(ed_time_cost.size() > 0)
	{
		ed_time_cost[0].time = min_driv_time.GetMinTime(ed_time_cost[0].a, ed_time_cost[0].b, start_time);
		ed_time_cost[0].departure_time = start_time;
		common_time += ed_time_cost[0].time;
		int tmp = 0;
		for(int i = 1; i < ed_time_cost.size(); i++)
		{
			tmp = min_driv_time.GetMinTime(ed_time_cost[i].a, ed_time_cost[i].b, common_time);
			ed_time_cost[i].time = tmp < 0 ? INT_MAX : tmp; 
			if(ed_time_cost[i].time < 0 || ed_time_cost[i].time > 10000)
			{
				cout<<"error"<<endl;
			}
			ed_time_cost[i].departure_time = common_time;
			common_time += ed_time_cost[i].time;
		}

		time_in_way = common_time - start_time;
	}

	return 0;
}

int Bus::MinimalCostsCalculation(EdgeCost min_base_costs)	//расчет стоимости проезда по ребрам маршрута
{
	cost = 0;
	if(bus_stops.size() <= 0)	//Если нет остановок
	{
		cout<<"Error! Bus stops not found."<<endl;
		return -1;
	}

	int tmp = 0;
	for(int i = 0; i < ed_time_cost.size(); i++)
	{
		tmp = lambda * min_base_costs.GetEdgeCost(ed_time_cost[i].a, ed_time_cost[i].b, ed_time_cost[i].departure_time);
		ed_time_cost[i].cost = tmp < 0 ? INT_MAX : tmp;
		cost += ed_time_cost[i].cost;
	}

	return 0;
}


Bus::~Bus(void)
{
}
