#pragma once

#include <vector>
using namespace std;

class BusStopsOnRoutes
{
private:

	struct bus
	{
		int rout_label;	
		int capacity;
		int empty_seats;
		int busy_seats;
		int local_fine;

		vector<pair<int, int>> intention;
	};

	struct bus_stop
	{
		int label;
		vector <bus> buses;
	};

	int stops_number;
	vector<bus_stop> bus_stops;
	
public:
	BusStopsOnRoutes(void);
	~BusStopsOnRoutes(void);
};

