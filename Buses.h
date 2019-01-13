#pragma once

#include "Bus.h"
#include <vector>
#include <algorithm>
using namespace std;

class Buses
{
public:
	vector <Bus> buses;
	Buses(void);
	int BusesInit(Routes routes, Individual individ);
	int TimesAndCostsCalc(MinimalDriveTime mindrivetime, EdgeCost minbcost);	//������ ������ � ���������� ����������� ���������� �� ���������
	double BusesAllCosts();	//������ ���������� ������� ��������� �� �� ������
	double BusesAllRemainPassangers();	//������ ������� ����������� ��������� �� �� ������
	int PrintBusList();	//����� ���������� �� ���������
	int EventsCreate(Events &ev);	//�������� ������ ������� �������� ��������� �� ���������
	~Buses(void);
};

