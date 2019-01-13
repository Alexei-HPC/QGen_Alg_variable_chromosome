#pragma once
#include <vector>
#include "tinyxml.h"
#include <fstream>
#include <algorithm> 

using namespace std;

class MinimalDriveTime
{
private:
	struct edge
	{
		unsigned long a;
		unsigned long b;
		vector <pair <int, double>> time_drivetime;
	};

	vector <edge> time_graph;
	double minimal_drive_time;

public:
	MinimalDriveTime(void);
	MinimalDriveTime(string path, double min_drive_time);
	int MinDriveTimeInit(string path);
	int GetMinTime(unsigned long a, unsigned long b, int time);
	int MinTimesPrint();
	~MinimalDriveTime(void);
};

