#pragma once

#include <vector>

using namespace std;

struct bus
{
	int capacity;	//���������������
	double lambda;	//����������� ������ �� ����������� ����� ��������
	int bus_label;	//����� ��������
};

class Rout
{

public:
	int max_interval;
	int trip_numbers; //����� ������ == ����� ���������
    int rout_label; //����� ��������
	pair <int, int> gap; //����� ������ ��������
	vector <unsigned long> stops; //������ �������� ��������
	vector <bus> buses;//�������� capacity lambda
	Rout(void);
	~Rout(void);
};

