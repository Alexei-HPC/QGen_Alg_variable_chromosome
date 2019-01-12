#pragma once
#include "Event.h"
#include <vector>
using namespace std;

class Events
{
	
public:
	vector <Event> events_vector;	//¬ектор событий прибыти€ автобусов на остановки отсортированный по времени прибыти€

	Events(void);
	int PrintEvents();	//¬ывод информации о событи€х
	~Events(void);
};

