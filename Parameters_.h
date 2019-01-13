#pragma once
#include <string>
#include <mpi.h>
#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

class Parameters_
{
public:
	int genetic_alg_iterations;// Число итераций генетического алгоритма
	int individ_number;//Число особей
	int chromosome_size;//Длина хромосомы, количество генов
	double crossover_probability;//Вероятность скрещивания
	double mutation_probability;//Вероятность мутации
	int mutation_number;//Число мутирующих генов
	int exchange_period;//Период после которого происходит обмен особями
	int exchange_individs;//Число особей для обмена
	int mutation_period;//Период, через который происходит мутация
	int truncation;//Величина усечения
	string density_filename;	//Путь к файлу с данными о приходах пассажиров на остановки
	string times_filename;	//Путь к файлу с данными о времни, которое необходимо для преодоления пути между остановками
	string costs_filename;	//Путь к файлу с данными о ценах проезда автобуса между остановками в различное время
	string routes_filename;	//Путь к файлу с данными о маршрутах
	string parameters_filename; //Путь к файлу с настройками
	string uni_path; //Путь к расшаренной папке

	int fine_for_remain_passanger;
	int fine_for_lag;
	int fine_for_last_remain_passangers;

	int wt_no_fine;	//интервал "бесплатного" ожидания (если пассажир ждёт меньше или равно этого времени, но НЕ штрафуем);
	int wt_restful;	//интервал "относительно спокойного" ожидания (мерится от момента прихода пассажира на остановку);
	int rm_restful;	//ставка штрафа за минуту ожидания сверх "бесплатного",  но не более "относительно спокойного";
	int wt_max;	//интервал  "максимального" ожидания (когда этот интервал заканчивается, то пассажиру надоедает ждать и он отказывается от поездки), мерится от момента прихода пассажира на остановку;
	int rm_upto_max;	//ставка штрафа за минуту ожидания сверх "относительно спокойного" интервала ожидания,  но не более "максимального";
	int f_refuse;	//ставка штрафа за отказ пассажира от поездки.

	int Parameters_Read(string path);	//Считывание параметров
	int Parameters_Bcasting();//Рассылка параметров между узлами
	int Print();//Вывод параметров
	Parameters_(void);
	~Parameters_(void);
};

