#pragma once

#include <vector>
#include <iostream>
#include "EdgeCost.h"
#include "MinimalDriveTime.h"
#include "Routes.h"
#include "Individual.h"
#include "Events.h"
#include "Passanger.h"

using namespace std;

class Bus
{
	struct edge
	{
		unsigned long a;	//������ �����
		unsigned long b;	//����� �����

		int departure_time;	//����� ������ ���� �� ������� �����
		int time;	//����� �� ����������� �����
		int cost;	//���� ����������� �����
	};

public:
	int my_rout_max_interval;	//������������ ����������� �� �������� �������� ����� ����������
	int bus_label;	//����� ��������
	int capacity;	//��������������� ��������
	int free_places;//���������� ��������� ����
	int rout_label;	//����� ��������

	int start_time;	//����� ������ �������� � ����
	int time_in_way;//����� �������� � ����

	double lambda;	//����������� ������ �� ����������� ����� ���������

	//vector < pair <unsigned long, int> > intentions;	//������ ���������� ����������(first) ������ �� �����������(second)
	vector <Passanger> passangers;
	vector < unsigned long > bus_stops;	//��������� ��������, ������������� 
	vector < edge > ed_time_cost;	

	double passangers_wt_fines;
	double remain_passangers;	//������
	double cost;	//����

	Bus(void);
	Bus(int rout_label, int bus_label, int capacity, double lambda);
	int PrintBusInfo();	//����� ���������� �� ��������
	int GetBusEvents(Events &events_vector);
	int TimeInit(int time);	//������������� ������� ������ �������� � ����
	int RoutInit(vector < unsigned long > stops);	//������������� ���������
	int MinimalTimesCalculation(MinimalDriveTime min_driv_time);	//������ ������� ����������� �������� �� ������
	int MinimalCostsCalculation(EdgeCost min_base_costs);	//������ ���������� ������� �������� �� ������
	~Bus(void);
};

