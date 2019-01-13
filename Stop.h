#pragma once
#include <vector>
#include "tinyxml.h"
#include <algorithm>
#include <fstream>
#include "Passanger.h"
#include "Bus.h"
#include "Parameters_.h"

using namespace std;

struct intentions
{
	unsigned long direction;	//�����������
	int passangers_number;	//����� ����������
};	

struct timedensity
{
	int time;	//�����
	int density_val;	//����� ��������� ����������
};

struct dir_td
{
	unsigned long dir;	//�����������
	vector <timedensity> vtimedens; //������� ���������� �� �������
};

class Stop
{
public:
	unsigned long bus_stop_label; //����� ���������
	int total_passangers; //����� ����� ���������� �� ���������
	//vector<intentions> remain_passangers; //��������� ������� �� ��������� + �� �����������
	vector< Passanger > passangers;//���������, ������� �� ���������, � ��� ���������� � ���
	int time_of_last_rout; //�����, ����� �������� ������� �������
	vector<dir_td> vdensity; //���� < ����������� ������ <�����_���������>>

	int Recalc(int current_time); //��������� ���������� ���������� �� ������� ���������
	int landLoadPassangers(/*vector <pair<unsigned long, int>> &bus_intentions*/ vector<Passanger> &bus_pussangers, int &free_places, int current_time, vector <unsigned long> bus_stops, Parameters_ parameters, Bus &bus);	//������� � ������� ���������� �� ���������
	int RemainPassangers(); //���������� ���������� ���������� ��������� �� ����� ��� ����� ���������� ��������
	int Print();	//����� ���������� � ���������
	Stop(void);
	Stop(unsigned long pbus_stop_label);
	~Stop(void);
};

