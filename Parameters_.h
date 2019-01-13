#pragma once
#include <string>
#include <mpi.h>
#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

class Parameters_
{
public:
	int genetic_alg_iterations;// ����� �������� ������������� ���������
	int individ_number;//����� ������
	int chromosome_size;//����� ���������, ���������� �����
	double crossover_probability;//����������� �����������
	double mutation_probability;//����������� �������
	int mutation_number;//����� ���������� �����
	int exchange_period;//������ ����� �������� ���������� ����� �������
	int exchange_individs;//����� ������ ��� ������
	int mutation_period;//������, ����� ������� ���������� �������
	int truncation;//�������� ��������
	string density_filename;	//���� � ����� � ������� � �������� ���������� �� ���������
	string times_filename;	//���� � ����� � ������� � ������, ������� ���������� ��� ����������� ���� ����� �����������
	string costs_filename;	//���� � ����� � ������� � ����� ������� �������� ����� ����������� � ��������� �����
	string routes_filename;	//���� � ����� � ������� � ���������
	string parameters_filename; //���� � ����� � �����������
	string uni_path; //���� � ����������� �����

	int fine_for_remain_passanger;
	int fine_for_lag;
	int fine_for_last_remain_passangers;

	int wt_no_fine;	//�������� "�����������" �������� (���� �������� ��� ������ ��� ����� ����� �������, �� �� ��������);
	int wt_restful;	//�������� "������������ ����������" �������� (������� �� ������� ������� ��������� �� ���������);
	int rm_restful;	//������ ������ �� ������ �������� ����� "�����������",  �� �� ����� "������������ ����������";
	int wt_max;	//��������  "�������������" �������� (����� ���� �������� �������������, �� ��������� ��������� ����� � �� ������������ �� �������), ������� �� ������� ������� ��������� �� ���������;
	int rm_upto_max;	//������ ������ �� ������ �������� ����� "������������ ����������" ��������� ��������,  �� �� ����� "�������������";
	int f_refuse;	//������ ������ �� ����� ��������� �� �������.

	int Parameters_Read(string path);	//���������� ����������
	int Parameters_Bcasting();//�������� ���������� ����� ������
	int Print();//����� ����������
	Parameters_(void);
	~Parameters_(void);
};

