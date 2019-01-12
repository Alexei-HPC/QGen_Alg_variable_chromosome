#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Event
{
	
public:
	static bool Compair(Event a, Event b);
	int bus_label;	//����� ��������
	int rout_label;	//����� ��������
	unsigned long bus_stop_label;	//����� ���������
	int rout_max_interval;	//������������ �������� ����� ���������� �� �������� �������� �������� 
	int time;	//����� �������� �������� �� ���������

	Event(void);
	Event(int bus_label, int rout_label, unsigned long bus_stop_label, int time);
	int Print(); //����� ���������� � ������� �������� �������� �� ���������
	~Event(void);	
};

