#include <QCoreApplication>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <set>
#include <mpi.h>
#include <sstream>

#include "Population.h"
#include "Individual.h"

#include "EdgeCost.h"
#include "MinimalDriveTime.h"
#include "Rout.h"
#include "Routes.h"
#include "Bus.h"
#include "Buses.h"
#include "Stop.h"
#include "Event.h"
#include "Events.h"
#include "Parameters_.h"

using namespace std;

int com_size;
int proc_rank;

//typedef struct //Параметры проведения экспериментов
//{
//	int genetic_alg_iterations;// Число итераций генетического алгоритма
//	int individ_number;//Число особей
//	int chromosome_size;//Длина хромосомы, количество генов
//	double crossover_probability;//Вероятность скрещивания
//	double mutation_probability;//Вероятность мутации
//	int mutation_number;//Число мутирующих генов
//	int exchange_period;//Период после которого происходит обмен особями
//	int exchange_individs;//Число особей для обмена
//	int mutation_period;//Период, через который происходит мутация
//	int truncation;//Величина усечения
//	string density_filename;	//Путь к файлу с данными о приходах пассажиров на остановки
//	string times_filename;	//Путь к файлу с данными о времни, которое необходимо для преодоления пути между остановками
//	string costs_filename;	//Путь к файлу с данными о ценах проезда автобуса между остановками в различное время
//	string routes_filename;	//Путь к файлу с данными о маршрутах
//	string parameters_filename; //Путь к файлу с настройками
//	string uni_path; //Путь к расшаренной папке
//
//	int fain_for_remain_passangers;
//	int fain_for_lag;
//	int fine_for_last_remain_passangers;
//
//	int wt_no_fine;	//интервал "бесплатного" ожидания (если пассажир ждёт меньше или равно этого времени, но НЕ штрафуем);
//	int wt_restful;	//интервал "относительно спокойного" ожидания (мерится от момента прихода пассажира на остановку);
//	int rm_restful;	//ставка штрафа за минуту ожидания сверх "бесплатного",  но не более "относительно спокойного";
//	int wt_max;	//интервал  "максимального" ожидания (когда этот интервал заканчивается, то пассажиру надоедает ждать и он отказывается от поездки), мерится от момента прихода пассажира на остановку;
//	int rm_upto_max;	//ставка штрафа за минуту ожидания сверх "относительно спокойного" интервала ожидания,  но не более "максимального";
//	int f_refuse;	//ставка штрафа за отказ пассажира от поездки.
//}Parameters;
//
//int Parameters_Read(string path, Parameters &parameters)	//Считывание параметров
//{
//	cout<<"Parameters reading started"<<endl;
//	TiXmlDocument doc;
//	if(!doc.LoadFile(path))//Попытка открыть файл
//	{
//		cerr << doc.ErrorDesc() << endl;
//		return 1;
//	}
//
//	TiXmlElement* root = doc.FirstChildElement();
//	if(root == NULL)
//	{
//		cerr << "Failed to load file: No root element."	<< endl;
//		doc.Clear();
//		return 1;
//	}
//
//	parameters.genetic_alg_iterations = atoi(root->FirstChildElement("genetic_alg_iterations")->GetText());
//	parameters.individ_number = atoi(root->FirstChildElement("individ_number")->GetText());
//	parameters.chromosome_size = atoi(root->FirstChildElement("chromosome_size")->GetText());
//	parameters.crossover_probability = atof(root->FirstChildElement("crossover_probability")->GetText());
//	parameters.mutation_probability = atof(root->FirstChildElement("mutation_probability")->GetText());
//	parameters.mutation_number = atoi(root->FirstChildElement("mutation_number")->GetText());
//	parameters.truncation = atoi(root->FirstChildElement("truncation")->GetText());
//	parameters.exchange_individs = atoi(root->FirstChildElement("exchange_individs")->GetText());;
//	parameters.exchange_period = atoi(root->FirstChildElement("exchange_period")->GetText());
//	parameters.mutation_period = atoi(root->FirstChildElement("mutation_period")->GetText());
//	parameters.fain_for_remain_passangers = atoi(root->FirstChildElement("fine_for_remain_passanger")->GetText());
//	parameters.fain_for_lag = atoi(root->FirstChildElement("fine_for_lag")->GetText());
//	parameters.fine_for_last_remain_passangers = atoi(root->FirstChildElement("fine_for_last_remain_passangers")->GetText());
//	//Чтение штрафов за ожидание пассажиров сверх нормы и т п а также соответствующие нормы по времени
//	parameters.wt_no_fine = atoi(root->FirstChildElement("wt_no_fine")->GetText());
//	parameters.wt_restful = atoi(root->FirstChildElement("wt_restful")->GetText());
//	parameters.wt_max = atoi(root->FirstChildElement("wt_max")->GetText());
//
//	parameters.rm_restful = atoi(root->FirstChildElement("rm_restful")->GetText());;
//	parameters.rm_upto_max = atoi(root->FirstChildElement("rm_upto_max")->GetText());;
//	parameters.f_refuse= atoi(root->FirstChildElement("f_refuse")->GetText());;
//
//	//Чтение путей к файлам
//	//parameters.times_filename = root->FirstChildElement("times_filename")->GetText();
//	parameters.density_filename = root->FirstChildElement("density_filename")->GetText();
//	parameters.routes_filename = root->FirstChildElement("routes_filename")->GetText();
//	parameters.costs_filename = root->FirstChildElement("costs_filename")->GetText();
//
//	cout<<"Parameters reading completed"<<endl;
//	return 0;
//}


bool Comparator(dir_td a, dir_td b) //Для сортировки направлений
{
    return (a.dir < b.dir);
}


bool Compar2(timedensity a, timedensity b) //Для сортировки времени
{
    return (a.time < b.time);
}


int LoadStops(string path, vector<Stop> &stops) //Загрузка плотностей пассажиров по времени
{
    TiXmlDocument doc;
    if(!doc.LoadFile(path))//Попытка открыть файл
    {
        cerr << doc.ErrorDesc() << endl;
        return 1;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if(root == NULL)
    {
        cerr << "Failed to load file: No root element."	<< endl;
        doc.Clear();
        return 1;
    }

    stops.clear();
    int stop_label;
    int direction;
    int time;
    double pass_dens;

    for(TiXmlElement* stop_it = root->FirstChildElement(); stop_it != NULL; stop_it = stop_it->NextSiblingElement())
    {
        Stop st;
        stop_label = atoi(stop_it->Attribute("stop_label"));
        st.bus_stop_label = stop_label;
        for(TiXmlElement* direction_it = stop_it->FirstChildElement(); direction_it != NULL; direction_it = direction_it->NextSiblingElement())
        {
            vector <timedensity> dens;
            direction = atoi(direction_it->Attribute("direction"));
            for(TiXmlElement* time_it = direction_it->FirstChildElement(); time_it != NULL; time_it = time_it->NextSiblingElement())
            {
                timedensity dn;
                dn.time = atoi(time_it->Attribute("time_val"));
                dn.density_val = atof(time_it->Attribute("passanger_number"));
                dens.push_back(dn);
            }
            dir_td temp_dtd;
            temp_dtd.dir = direction;
            temp_dtd.vtimedens = dens;
            st.vdensity.push_back(temp_dtd);
        }
        stops.push_back(st);
    }
    for(int i = 0; i < stops.size(); i++)
    {

        for(int j = 0; j < stops[i].vdensity.size(); j++)
        {
            sort(stops[i].vdensity.begin(), stops[i].vdensity.end(), Comparator);//Сортируем по направлениям

            for(int k = 0; k < stops[i].vdensity[j].vtimedens.size(); k++)
            {
                sort(stops[i].vdensity[j].vtimedens.begin(), stops[i].vdensity[j].vtimedens.end(), Compar2); //Сортируем по времени
            }
        }
    }
    return 0;
}


int RingSending(int size, int* arraytosend, int* arraytorecv)
{
    int my_proc_rank = 0;
    int my_com_size = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &my_com_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_proc_rank);

    MPI_Status status;
    //Определяем кому отправим и от кого примем
    int to = my_proc_rank + 1;
    int from = my_proc_rank - 1;
    if(to >= my_com_size)
    {
        to = 0;
    }
    if(from < 0)
    {
        from = my_com_size - 1;
    }

    cout<<"________________________Exchanging started. Procrank: "<<my_proc_rank<<endl;

    if(my_proc_rank % 2 == 0)
    {
        cout<<"In SR. procrank: "<<my_proc_rank<<endl;

        cout<<"Sending. Procrank "<<my_proc_rank<<" size: "<<size<<" to: "<<to<<endl;
        MPI_Send(arraytosend, size, MPI_INT, to, to, MPI_COMM_WORLD);
        cout<<"-------------------------------Sending successful. Procrank "<<my_proc_rank<<" size: "<<size<<" to: "<<to<<endl;

    }
    else
    {
        cout<<"In RS. procrank: "<<my_proc_rank<<endl;
        cout<<"Receiving. Procrank "<<my_proc_rank<<" size: "<<size<<" from: "<<from<<endl;
        MPI_Recv(arraytorecv, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<my_proc_rank<<" size: "<<size<<" from: "<<status.MPI_SOURCE<<endl;
    }

    cout<<"Im "<<my_proc_rank<<" before barrier"<<endl;
    //MPI_Barrier(MPI_COMM_WORLD);//Убрать после отладки!
    cout<<"Im "<<my_proc_rank<<" after barrier"<<endl;

    if(my_proc_rank % 2 == 0)
    {
        cout<<"Receiving. Procrank "<<my_proc_rank<<" size: "<<size<<" from: "<<from<<endl;
        MPI_Recv(arraytorecv, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<my_proc_rank<<" size: "<<size<<" from: "<<status.MPI_SOURCE<<endl;
    }
    else
    {
        cout<<"Sending. Procrank "<<my_proc_rank<<" size: "<<size<<" to: "<<to<<endl;
        MPI_Send(arraytosend, size, MPI_INT, to, to, MPI_COMM_WORLD);
        cout<<"--------------------------------Sending successful. Procrank "<<my_proc_rank<<" size: "<<size<<" to: "<<to<<endl;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    cout<<"________________________Exchanging finished. Procrank: "<<my_proc_rank<<endl;
    //MPI_Barrier(MPI_COMM_WORLD);//Убрать после отладки!

    return 0;
}

int Exchange(Population &population, Parameters_ parameters)//Кольцевой обмен особями между узлами
{
    cout<<"______________________Exchanging started. Procrank: "<<proc_rank<<endl;
    if(com_size == 1)//если только 1 узел в сети
    {
        cout<<"________________________Exchanging finished becuse only one node. Procrank: "<<proc_rank<<endl;
        return 0;
    }


    int size = parameters.exchange_individs * parameters.chromosome_size;

    //Массивы под прием и отправку хромосом
    int* tempsendarr = new int[size];
    int* temprecvarr = new int[size];

    int index = 0;

    index = 0;

    cout<<"Prepairing data"<<endl;
    //Копируем хромосомы в массив для отправки
    for(int i = 0; i < parameters.exchange_individs; i++)
    {
        for(int k = 0; k < population.chromosome_size; k++)
        {
            tempsendarr[index] = population.individuals[i].chromosome[k];
            index++;
        }
    }

    cout<<"RingSending"<<endl;
    RingSending(size, tempsendarr, temprecvarr);

    cout<<"Copying data to chromosomes"<<endl;
    index = 0;
    //Копируем гены из принятого массива в наши хромосомы
    for(int i = 0; i < parameters.exchange_individs; i++)
    {
        for(int k = 0; k < population.chromosome_size; k++)
        {
            population.individuals[i].chromosome[k] = temprecvarr[index];
            index++;
        }
    }

    delete [] temprecvarr;
    delete [] tempsendarr;

    cout<<"________________________Exchanging finished. Procrank: "<<proc_rank<<endl;
    //MPI_Barrier(MPI_COMM_WORLD);//Убрать после отладки!

    return 0;
}

/*
int Exchange(Population &population, Parameters parameters)//Кольцевой обмен особями между узлами
{
    cout<<"______________________Exchanging started. Procrank: "<<proc_rank<<endl;
    if(com_size == 1)//если только 1 узел в сети
    {
        cout<<"________________________Exchanging finished becuse only one node. Procrank: "<<proc_rank<<endl;
        return 0;
    }

    MPI_Status status;
    //Определяем кому отправим и от кого примем
    int to = proc_rank + 1;
    int from = proc_rank - 1;
    if(to >= com_size)
    {
        to = 0;
    }
    if(from < 0)
    {
        from = com_size - 1;
    }

    //cout<<"..........................Im "<<proc_rank<<" from: "<<from<<" to: "<<to<<" parameters.exchange_individs: "<<parameters.exchange_individs<<" parameters.chromosome_size: "<<parameters.chromosome_size<<endl;
    int size = parameters.exchange_individs * parameters.chromosome_size;

    //Массивы под прием и отправку хромосом
    int* tempsendarr = new int[size];
    int* temprecvarr = new int[size];

    int index = 0;

    //for(int i = 1; i < com_size - 1; i++)
    //{
    //	if(i == proc_rank)
    //	{
    //		int next = i + 1;

    //		index = 0;
    //		//Копируем хромосомы в массив для отправки
    //		for(int i = 0; i < parameters.exchange_individs; i++)
    //		{
    //			for(int k = 0; k < population.chromosome_size; k++)
    //			{
    //				tempsendarr[index] = population.individuals[i].chromosome[k];
    //				index++;
    //			}
    //		}

    //		cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<next<<endl;
    //		MPI_Send(tempsendarr, size, MPI_INT, next, 0, MPI_COMM_WORLD);
    //		cout<<"-------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<next<<endl;

    //		cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
    //		MPI_Recv(temprecvarr, size, MPI_INT, next, 0, MPI_COMM_WORLD, &status);
    //		cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<next<<endl;
    //		//cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<com_size - 1<<endl;
    //		//MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //		index = 0;
    //		//Копируем гены из принятого массива в наши хромосомы
    //		for(int i = 0; i < parameters.exchange_individs; i++)
    //		{
    //			for(int k = 0; k < population.chromosome_size; k++)
    //			{
    //				population.individuals[i].chromosome[k] = temprecvarr[index];
    //				index++;
    //			}
    //		}

    //	}
    //	if(i == proc_rank + 1)
    //	{
    //		int prev = i - 1;

    //		cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<prev<<endl;
    //		MPI_Recv(temprecvarr, size, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
    //		cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<prev<<endl;
    //		index = 0;
    //		//Копируем хромосомы в массив для отправки
    //		for(int i = 0; i < parameters.exchange_individs; i++)
    //		{
    //			for(int k = 0; k < population.chromosome_size; k++)
    //			{
    //				tempsendarr[index] = population.individuals[i].chromosome[k];
    //				index++;
    //			}
    //		}
    //		//cout<<"Im "<<proc_rank<<" to "<<0<<" from "<<0<<endl;
    //		//MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //		cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<prev<<endl;
    //		MPI_Send(tempsendarr, size, MPI_INT, prev, 0, MPI_COMM_WORLD);
    //		cout<<"--------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<prev<<endl;
    //		index = 0;
    //		for(int i = 0; i < parameters.exchange_individs; i++)
    //		{
    //			for(int k = 0; k < population.chromosome_size; k++)
    //			{
    //				population.individuals[i].chromosome[k] = temprecvarr[index];
    //				index++;
    //			}
    //		}

    //	}
    //}

    //if(proc_rank == 0)
    //{
    //	index = 0;
    //	//Копируем хромосомы в массив для отправки
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			tempsendarr[index] = population.individuals[i].chromosome[k];
    //			index++;
    //		}
    //	}

    //	cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<com_size - 1<<endl;
    //	MPI_Send(tempsendarr, size, MPI_INT, com_size - 1, 0, MPI_COMM_WORLD);
    //	cout<<"-------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<com_size - 1<<endl;

    //	cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
    //	MPI_Recv(temprecvarr, size, MPI_INT, com_size - 1, 0, MPI_COMM_WORLD, &status);
    //	cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<com_size - 1<<endl;
    //	//cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<com_size - 1<<endl;
    //	//MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //	index = 0;
    //	//Копируем гены из принятого массива в наши хромосомы
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			population.individuals[i].chromosome[k] = temprecvarr[index];
    //			index++;
    //		}
    //	}

    //}

    //if(proc_rank == com_size - 1)
    //{
    //	cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<com_size - 1<<endl;
    //	MPI_Recv(temprecvarr, size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    //	cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<com_size - 1<<endl;
    //	index = 0;
    //	//Копируем хромосомы в массив для отправки
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			tempsendarr[index] = population.individuals[i].chromosome[k];
    //			index++;
    //		}
    //	}
    //	//cout<<"Im "<<proc_rank<<" to "<<0<<" from "<<0<<endl;
    //	//MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //	cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<0<<endl;
    //	MPI_Send(tempsendarr, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    //	cout<<"--------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<0<<endl;
    //	index = 0;
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			population.individuals[i].chromosome[k] = temprecvarr[index];
    //			index++;
    //		}
    //	}
    //}
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////Копируем хромосомы в массив для отправки
    //for(int i = 0; i < parameters.exchange_individs; i++)
    //{
    //	for(int k = 0; k < population.chromosome_size; k++)
    //	{
    //		tempsendarr[index] = population.individuals[i].chromosome[k];
    //		index++;
    //	}
    //}

    //cout<<"Array to send: "<<endl;
    //for(int i = 0; i < size; i++)
    //{
    //	cout<<tempsendarr[i]<<" ";
    //}
    //cout<<endl;

    //cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<from<<endl;
    //MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //index = 0;
    ////Копируем гены из принятого массива в наши хромосомы
    //for(int i = 0; i < parameters.exchange_individs; i++)
    //{
    //	for(int k = 0; k < population.chromosome_size; k++)
    //	{
    //		population.individuals[i].chromosome[k] = temprecvarr[index];
    //		index++;
    //	}
    //}

    //cout<<"received array: "<<endl;
    //for(int i = 0; i < size; i++)
    //{
    //	cout<<temprecvarr[i]<<" ";
    //}
    //cout<<endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //if(proc_rank % 2 == 0)
    //{
    //	cout<<"In SR. procrank: "<<proc_rank<<endl;
    //	index = 0;
    //	//Копируем хромосомы в массив для отправки
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			tempsendarr[index] = population.individuals[i].chromosome[k];
    //			index++;
    //		}
    //	}

    //	cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;
    //	MPI_Send(tempsendarr, size, MPI_INT, to, to, MPI_COMM_WORLD);
    //	cout<<"-------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;

    //	cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
    //	MPI_Recv(temprecvarr, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //	cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
    //	cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<status.MPI_SOURCE<<endl;
    //	//MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //	index = 0;
    //	//Копируем гены из принятого массива в наши хромосомы
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			population.individuals[i].chromosome[k] = temprecvarr[index];
    //			index++;
    //		}
    //	}
    //}
    //else
    //{
    //	cout<<"In RS. procrank: "<<proc_rank<<endl;
    //	cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
    //	MPI_Recv(temprecvarr, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //	cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<status.MPI_SOURCE<<endl;
    //	index = 0;
    //	//Копируем хромосомы в массив для отправки
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			tempsendarr[index] = population.individuals[i].chromosome[k];
    //			index++;
    //		}
    //	}
    //	cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<from<<endl;
    //	//MPI_Sendrecv(tempsendarr, size, MPI_INT, to, 0, temprecvarr, size, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

    //	cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;
    //	MPI_Send(tempsendarr, size, MPI_INT, to, to, MPI_COMM_WORLD);
    //	cout<<"--------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;
    //	index = 0;
    //	for(int i = 0; i < parameters.exchange_individs; i++)
    //	{
    //		for(int k = 0; k < population.chromosome_size; k++)
    //		{
    //			population.individuals[i].chromosome[k] = temprecvarr[index];
    //			index++;
    //		}
    //	}
    //}
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(proc_rank % 2 == 0)
    {
        cout<<"In SR. procrank: "<<proc_rank<<endl;
        index = 0;
        //Копируем хромосомы в массив для отправки
        for(int i = 0; i < parameters.exchange_individs; i++)
        {
            for(int k = 0; k < population.chromosome_size; k++)
            {
                tempsendarr[index] = population.individuals[i].chromosome[k];
                index++;
            }
        }

        cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;
        MPI_Ssend(tempsendarr, size, MPI_INT, to, to, MPI_COMM_WORLD);
        cout<<"-------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;

    }
    else
    {
        cout<<"In RS. procrank: "<<proc_rank<<endl;
        cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
        MPI_Recv(temprecvarr, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<status.MPI_SOURCE<<endl;
        index = 0;
        //Копируем хромосомы в массив для отправки
        for(int i = 0; i < parameters.exchange_individs; i++)
        {
            for(int k = 0; k < population.chromosome_size; k++)
            {
                tempsendarr[index] = population.individuals[i].chromosome[k];
                index++;
            }
        }
        cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<from<<endl;
    }
    cout<<"Im "<<proc_rank<<" before barrier"<<endl;
    MPI_Barrier(MPI_COMM_WORLD);//Убрать после отладки!
    cout<<"Im "<<proc_rank<<" after barrier"<<endl;

    if(proc_rank % 2 == 0)
    {
        cout<<"Receiving. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<from<<endl;
        MPI_Recv(temprecvarr, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout<<"++++++++++++++++++++++++++++++Receiving successful. Procrank "<<proc_rank<<" size: "<<size<<" from: "<<status.MPI_SOURCE<<endl;
        cout<<"Im "<<proc_rank<<" to "<<to<<" from "<<from<<endl;

        index = 0;
        //Копируем гены из принятого массива в наши хромосомы
        for(int i = 0; i < parameters.exchange_individs; i++)
        {
            for(int k = 0; k < population.chromosome_size; k++)
            {
                population.individuals[i].chromosome[k] = temprecvarr[index];
                index++;
            }
        }
    }
    else
    {
        cout<<"Sending. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;
        MPI_Ssend(tempsendarr, size, MPI_INT, to, to, MPI_COMM_WORLD);
        cout<<"--------------------------------Sending successful. Procrank "<<proc_rank<<" size: "<<size<<" to: "<<to<<endl;
        index = 0;
        for(int i = 0; i < parameters.exchange_individs; i++)
        {
            for(int k = 0; k < population.chromosome_size; k++)
            {
                population.individuals[i].chromosome[k] = temprecvarr[index];
                index++;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    delete [] temprecvarr;
    delete [] tempsendarr;

    cout<<"________________________Exchanging finished. Procrank: "<<proc_rank<<endl;
    //MPI_Barrier(MPI_COMM_WORLD);//Убрать после отладки!
    return 0;
}*/

Individual GetBestSol(Population population)//Поиск лучшего решения среди всех узлов
{
    double minff = INT_MAX;
    MPI_Status status;
    int index = 0;
    Individual tempind(population.chromosome_size, population.routs_gaps);
    int size = population.chromosome_size + 1;
    double * tempchromosome = new double[size];
    double * bestsol = new double[size];
    if(proc_rank == 0)
    {
        for(int i = 0; i < population.population_size; i++)//Поиск наилучшего решения в своих особях
        {
            if(population.individuals[i].fitness_value <= minff)
            {
                minff = population.individuals[i].fitness_value;
                index = i;
            }
        }

        for(int j = 0; j < population.chromosome_size; j++)
        {
            bestsol[j] = population.individuals[index].chromosome[j];
        }
        bestsol[population.chromosome_size] = minff;

        for(int i = 1; i < com_size - 1; i++)//Прием наилучших решений других узлов
        {
            MPI_Recv(tempchromosome, size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            if(tempchromosome[population.chromosome_size] <= minff)
            {
                minff = tempchromosome[population.chromosome_size];
                for(int j = 0; j < population.chromosome_size; j++)
                {
                    bestsol[j] = tempchromosome[j];
                }
                bestsol[population.chromosome_size] = minff;
            }
        }
    }
    else
    {
        for(int i = 0; i < population.population_size; i++)//Поиск наилучшего решения в своих особях
        {
            if(population.individuals[i].fitness_value <= minff)
            {
                minff = population.individuals[i].fitness_value;
                index = i;
            }
        }

        for(int i = 0; i < population.chromosome_size; i++)
        {
            tempchromosome[i] = population.individuals[index].chromosome[i];
        }
        tempchromosome[population.chromosome_size] = population.individuals[index].fitness_value;
        MPI_Send(tempchromosome, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);//Отправка на нулевой узел наилучшего решения
    }

    for(int i = 0; i < size - 1; i++)
    {
        tempind.chromosome[i] = bestsol[i];
    }
    tempind.fitness_value = bestsol[size - 1];

    delete [] tempchromosome;
    delete [] bestsol;

    return tempind;
}

int MakeEvent(Event ev, Buses &buses, vector <Stop> &stops, Parameters_ parameters)//Событие прихода автобуса на остановку
{
    //Формирование имен файла с информацией о событиях
    /*string label = "";
    string filename1 = " events_info.txt";
    char temp[30];
    itoa(proc_rank, temp, 10);
    label += (char*) temp;
    ofstream events_info(label + filename1, ios::app);

    events_info<<"Event bus_label: "<<ev.bus_label<<" rout_label: "<<ev.rout_label<<" bus_stop_label: "<<ev.bus_stop_label<<" time: "<<ev.time<<endl;*/

    for(int i = 0; i < stops.size(); i++)//Ищем нужную остановку
    {
        if(stops[i].bus_stop_label == ev.bus_stop_label)
        {
            for(int j = 0; j < buses.buses.size(); j++)//Ищем нужный автобус
            {
                if(buses.buses[j].bus_label == ev.bus_label && buses.buses[j].rout_label == ev.rout_label)
                {
                    //events_info<<"found bus. bus label: "<<buses.buses[j].bus_label<<" Rout_label: "<<buses.buses[j].rout_label<<"free places: "<<buses.buses[j].free_places<<endl;
                    //events_info<<"bus intentions before. Free places: "<<buses.buses[j].free_places<<endl;
                    /*for(int w = 0; w < buses.buses[j].intentions.size(); w++)
                    {
                    events_info<<"\tdir: "<<buses.buses[j].intentions[w].first<<" pas num: "<<buses.buses[j].intentions[w].second<<endl;
                    }*/
                    stops[i].landLoadPassangers(/*buses.buses[j].intentions*/ buses.buses[j].passangers, buses.buses[j].free_places, ev.time, buses.buses[j].bus_stops, parameters, buses.buses[j]);//Выполняем процедуру высадки посадки пассажиров
                    //events_info<<"bus intentions after. Free places: "<<buses.buses[j].free_places<<endl;
                    //for(int w = 0; w < buses.buses[j].intentions.size(); w++)
                    //{
                    //	events_info<<"\tdir: "<<buses.buses[j].intentions[w].first<<" pas num: "<<buses.buses[j].intentions[w].second<<endl;
                    //}

                    //Считаем число пассажиров, оставленных на остановке желающих проехать на тукущем ПТС
                    int remain_number = 0;

                    for(int k = 0; k < stops[i].passangers.size();k++)//Пассажиры на остановке
                    {
                        for(int l = 0; l < buses.buses[j].bus_stops.size(); l++)//Остановки автобуса
                        {
                            if(stops[i].passangers[k].direction == buses.buses[j].bus_stops[l])
                            {
                                remain_number++;
                            }
                        }
                    }
                    buses.buses[j].remain_passangers += remain_number;

                    //Считаем число пассажиров, оставленных на остановке желающих проехать на тукущем ПТС
                    //for(int k = 0; k < buses.buses[j].bus_stops.size(); k++)
                    //{
                    //	for(int l = 0; l < stops[i].remain_passangers.size(); l++)
                    //	{
                    //		if(stops[i].remain_passangers[l].direction == buses.buses[j].bus_stops[k])
                    //		{
                    //			remain_number += stops[i].remain_passangers[l].passangers_number;
                    //		}
                    //	}
                    //}
                    //buses.buses[j].remain_passangers += remain_number;

                    //events_info<<"bus stop remain passangers:"<<endl;
                    /*for(int w = 0; w < stops[i].remain_passangers.size(); w++)
                    {
                    events_info<<"\tdir: "<<stops[i].remain_passangers[w].direction<<" pas num: "<<stops[i].remain_passangers[w].passangers_number<<endl;
                    }*/
                }
            }
        }
    }

    //events_info.close();

    return 0;
}

int FindRemaindPassangers(vector <Stop> stops)
{
    int remain_passangers = 0;

    for(int i = 0; i < stops.size(); i++)
    {
        remain_passangers += stops[i].RemainPassangers();
    }

    return remain_passangers;
}


int MaxIntervalViolation(Events events, int fine_val)	//Возвращает штраф за превышение макимального интервала между автобусами
{
    int exceed_minutes = 0;
    int tmp_minutes = 0;
    for(int i = 0; i < events.events_vector.size() - 1; i++)
    {
        for(int j = i + 1; j < events.events_vector.size(); j++)
        {
            //ищем прибытие того же автобуса на следующую остановку
            if(events.events_vector[i].bus_label == events.events_vector[j].bus_label)
            {
                if(events.events_vector[i].rout_label == events.events_vector[j].rout_label)
                {
                    //Если время между событиями прихода автобусов больше положенного интервала
                    if(events.events_vector[j].time - events.events_vector[i].time > events.events_vector[i].rout_max_interval)
                    {
                        tmp_minutes = events.events_vector[j].time - events.events_vector[i].time - events.events_vector[i].rout_max_interval;
                        exceed_minutes += tmp_minutes * fine_val * fine_val * tmp_minutes;	//� азмер штрафа возрастает квадратично
                    }
                }
            }
        }
    }

    return exceed_minutes;
}

double FitnessFunktion(Parameters_ parameters, MinimalDriveTime mindrivetime, EdgeCost minbcost, Routes routes, Individual individ, vector <Stop> stops)	//расчет приспособленности особи
{
    double fitness_function_value = 0;

    //Формирование имен файла с информацией о штрафах
    //string label = "";
    //string filename1 = " fines_info.txt";
    //char temp[30];
    //itoa(proc_rank, temp, 10);
    //label += (char*) temp;
    //ofstream fines_info(label + filename1, ios::app);

    //for(int i = 0; i < individ.chromosome_size; i++)
    //{
    //	fines_info<<individ.chromosome[i]<<" ";
    //}
    //fines_info<<endl;

    Buses buses;
    buses.BusesInit(routes, individ);
    buses.TimesAndCostsCalc(mindrivetime, minbcost);//Цены в данный момент не считаются
    double driving_cost = 0;
    //driving_cost = buses.BusesAllCosts(); //Вычисляем первую часть фитнес функции. Суммируем стоимости прохождения автобусов по их маршрутам
    driving_cost = 0;
    fitness_function_value += driving_cost;
    //fines_info<<"all buses driving cost: "<<driving_cost<<endl;
    Events events;
    buses.EventsCreate(events);
    //buses.PrintBusList();

    for(int i = 0; i < events.events_vector.size(); i++)
    {
        MakeEvent(events.events_vector[i], buses, stops, parameters);
    }
    double remain_pas = 0;
    remain_pas = buses.BusesAllRemainPassangers();	//Вычисляем вторую часть фитнесс функции. Суммируем штрафы за невзятых пассажиров
    fitness_function_value += remain_pas * parameters.fine_for_remain_passanger;
    //fines_info<<"all buses fines for remain passangers: "<<remain_pas * 200<<endl;

    //Вычисляем штрафы за оставленных пассажиров после последнего автобуса
    int remain_passangers = FindRemaindPassangers(stops);
    //fines_info<<"Fine for remain passangers after last bus: "<<remain_passangers * 200<<endl;
    fitness_function_value += remain_passangers * parameters.fine_for_last_remain_passangers;

    //Вычисляем штрафы за превышение максимальных интервалов между автобусами
    int fine_for_exceed_minutes = 0;
    fine_for_exceed_minutes = MaxIntervalViolation(events, parameters.fine_for_lag);
    //fines_info<<"all buses fine for exceed minutes: "<<fine_for_exceed_minutes<<endl;
    fitness_function_value += fine_for_exceed_minutes;

    //Складываем штрафы каждого автобуса за превышения времени ожидания пассажиров
    for(int i = 0; i < buses.buses.size(); i++)
    {
        fitness_function_value += buses.buses[i].passangers_wt_fines;
    }

    //fines_info<<"Fitness value: "<<fitness_function_value<<endl<<endl<<endl;
    //fines_info.close();
    return fitness_function_value;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    double start_time = 0;
    double finish_time = 0;

    Parameters_ parameters;//Структура для хранения параметров алгоритма
    //parameters.parameters_filename = "parameters.xml";
    //"parameters.xml"
    /*if (argc !=4 )// если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
    {
        cout << "Error! Wrong arguments number." << endl;
        MPI_Finalize();
        return 0;
    }
    else*/
    {
        parameters.parameters_filename = "parameters.xml";
        parameters.times_filename = "MinimalTimes.xml";
        parameters.uni_path = "";
        //cout<<"parameter: "<<parameters.parameters_filename<<endl;
        //cout<<"times: "<<parameters.times_filename<<endl;
    }

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &com_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    cout<<"Procrank: "<<proc_rank<<" Number of nodes: "<<com_size<<endl;

    //раскомментировать в зависимости от того как запускается, srand(time(NULL)) если только 1 узел
    //srand(time(NULL));
    srand(proc_rank);

    bool setting_readed = false;
    setting_readed = parameters.Parameters_Read(parameters.parameters_filename);
    if(setting_readed == 1)
    {
        cout << "Error! File \"parameters.xml\" with settings not found."<<endl;
        MPI_Finalize();
        return 1;
    }
    parameters.Print();

    EdgeCost minbcost(parameters.costs_filename, 9);	//Считываем стоимости прохождения автобусов по ребрам
    MinimalDriveTime mindrivetime(parameters.times_filename, 3);	//Считываем времена проходжения автобусов по ребрам
    Routes routes(parameters.routes_filename);	//Считываем информацию о маршрутах
    vector< pair <int, pair<int, int>>> routs_gaps;
    routes.GetGaps(routs_gaps);
    int ch_size = routes.ChromosomeSize();
    parameters.chromosome_size = ch_size;

    Population population(parameters.individ_number, ch_size, routs_gaps);//Инициализация популяции
    //population.PrintPopulation();

    vector <Stop> stops;
    LoadStops(parameters.density_filename, stops);	//Считываем информацию о приходе пассажиров на остановку

    if(proc_rank == 0)//Замер времени начала работы алгоритма
    {
        start_time = MPI_Wtime();
    }

    //Формирование имен выходных файлов
    string label = "";
    string filename1 = "_average_ff.txt";
    //string filename2 = "_file_ff_by_ind.txt";
//  used deprecated cast
/*
    char temp[30];
    itoa(proc_rank, temp, 10);
    label += (char*) temp;
*/
//  instead:
    label = to_string(proc_rank);
    ofstream file_aver_ff(parameters.uni_path + label + filename1);
    //ofstream file_ff_by_ind(label + filename2);

    MPI_Barrier(MPI_COMM_WORLD);
    cout<<"procrank: "<<proc_rank<<" Working started."<<endl<<endl;

    for(int iter = 1; iter <= parameters.genetic_alg_iterations; iter++)//Шаги генетического алгоритма
    {
        cout<<iter<<" iteration of GA started. ProcRank: "<<proc_rank<<endl;
        if(iter % parameters.mutation_period  == 0)//Мутация
        {
            population.Mutation(parameters.mutation_probability, parameters.mutation_number);
            cout<<"Mutation was carried"<<endl;
        }
        population.Crossover(parameters.crossover_probability);//Скрещивание
        cout<<"Crossing was carried"<<endl;
        for(int ind = 0; ind < population.population_size; ind++)//Нахождение значения фитнесс функци для каждого индивида
        {
            population.individuals[ind].fitness_value = FitnessFunktion(parameters, mindrivetime, minbcost, routes, population.individuals[ind], stops);
        }

        /*Вывод данных в выходной файл*/
        /*file_ff_by_ind<<"iteration: "<<iter<<endl;
        for(int k = 0; k < population.population_size; k++)
        {
        for(int s = 0; s < population.chromosome_size; s++)
        {
        file_ff_by_ind<<population.individuals[k].chromosome[s]<<" ";
        }
        file_ff_by_ind<<population.individuals[k].fitness_value;
        file_ff_by_ind<<endl;
        }*/

        double aver_ff = 0;
        for(int k = 0; k < population.population_size; k++)//Вычиление среднего значения фитнесс функции
        {
            aver_ff += population.individuals[k].fitness_value;
        }
        aver_ff = (double)aver_ff/parameters.individ_number;
        file_aver_ff<<iter<<" "<<aver_ff<<endl;
        cout<<"Average fitness function "<<aver_ff<<endl;

        if( iter % parameters.exchange_period  == 0)//Кольцевой обмен
        {
            Exchange(population, parameters);
            cout<<"Circular exchange was carried"<<endl;
        }
        population.Selection(parameters.truncation);
        cout<<"There was a selection"<<endl;
        cout<<iter<<" iteration of GA finished. ProcRank: "<<proc_rank<<endl;
    }
    file_aver_ff.close();
    //file_ff_by_ind.close();
    cout<<"GA finished. Procrank: "<<proc_rank<<endl;
    //Поиск лучшего решения
    if(proc_rank == 0)
    {
        Individual res_individ(GetBestSol(population));
        cout<<"Solution: "<<endl;
        res_individ.PrintIndivid();
        cout<<"Fitness function: "<<res_individ.fitness_value<<endl;

        ofstream SolutionFile(parameters.uni_path + "Solution.txt");

        //Выводим решение в файл
        if(SolutionFile.is_open())//проверяем, открыт ли файл
        {
            finish_time = MPI_Wtime();
            SolutionFile<<"Time: "<<finish_time - start_time<<endl;
            cout<<"Bus routes count: "<<routes.routes.size();
            int counter = 0;
            for(int k = 0; k < routes.routes.size(); k++)
            {
                SolutionFile<<"\nRoute: "<<routes.routes[k].rout_label<<", trips count: "<<routes.routes[k].buses.size()<<endl;
                for(int t = 0; t < routes.routes[k].buses.size(); t++)
                {
                    SolutionFile<<res_individ.chromosome[counter]<<" ";
                    counter++;
                }
                cout<<"\n";
            }

            //SolutionFile<<"Solution: "<<endl;
            //for(int i = 0; i < res_individ.chromosome_size; i++ )
            //{
            //	SolutionFile<<res_individ.chromosome[i]<<'\t';
            //}
            //cout<<endl;
            //cout<<endl;
            SolutionFile<<"\nFitness function: "<<res_individ.fitness_value<<endl;
            SolutionFile.close();
        }
        else
        {
            cout<<"Error! Failed to open a file for output solutions."<<endl;
        }
    }
    else	//Главный узел ищет наилучшее решение на всех узлах
    {
        GetBestSol(population);
    }

    if(proc_rank == 0)//Замер времени завершения работы алгоритма
    {
        finish_time = MPI_Wtime();
        cout<<"Time: "<<finish_time - start_time<<endl;
    }

    MPI_Finalize();


    return a.exec();
}
