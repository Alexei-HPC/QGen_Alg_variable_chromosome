#pragma once
#include "Event.h"
#include <vector>
using namespace std;

class Events
{
	
public:
	vector <Event> events_vector;	//������ ������� �������� ��������� �� ��������� ��������������� �� ������� ��������

	Events(void);
	int PrintEvents();	//����� ���������� � ��������
	~Events(void);
};

