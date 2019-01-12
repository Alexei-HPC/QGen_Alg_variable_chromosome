#include "Event.h"


Event::Event(void)
{
}

Event::Event(int pbus_label, int prout_label, unsigned long pbus_stop_label, int ptime)
{
	bus_label = pbus_label;
	rout_label = prout_label;
	bus_stop_label = pbus_stop_label;
	time = ptime;
}

bool Event::Compair(Event a, Event b)
{
	return a.time < b.time;
}

//int Event::EventsInit(Buses bus_vect)
//{
//	int bus_label;
//	int rout_label;
//	events.clear();
//
//	for(int i = 0; i < bus_vect.buses.size(); i++)
//	{
//		bus_label = Bus::buses[i].bus_label;
//		rout_label = Bus::buses[i].rout_label;
//		for(int j = 0; j < Bus::buses[i].ed_time_cost.size(); j++)
//		{
//			Event ev;
//			ev.time = Bus::buses[i].ed_time_cost[j].departure_time;
//			ev.bus_label = bus_label;
//			ev.rout_label = rout_label;
//			ev.bus_stop_label = Bus::buses[i].ed_time_cost[j].a;
//			events.push_back(ev);
//		}
//	}
//
//	sort(events.begin(), events.end(), Compair);
//
//	return 0;
//}

int Event::Print()
{
	cout<<"Event:"<<endl;
	cout<<"Bus label: "<<bus_label<<" Rout label: "<<rout_label<<" Bus stop label: "<<bus_stop_label<<" Time: "<<time<<endl;
	cout<<endl;
	return 0;
}


Event::~Event(void)
{
}
