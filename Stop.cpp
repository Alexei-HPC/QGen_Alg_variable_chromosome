#include "Stop.h"
#include <fstream>


Stop::Stop(void)
{
	total_passangers = 0;
	bus_stop_label = 0;
	time_of_last_rout = 0;
}

Stop::Stop(unsigned long pbus_stop_label)
{
	bus_stop_label = pbus_stop_label; //Номер остановки
	total_passangers = 0; //Общее число пассажиров на остановке
}

//
//int Stop::landLoadPassangers(vector <pair<int, int>> &bus_intentions, int &free_places, int curent_time, vector <int> bus_stops)
//{
//	//Пассажиры выходят на нужной остановке
//	for(int i = 0; i < bus_intentions.size(); i++)
//	{
//		if(bus_intentions[i].first == bus_stop_label)
//		{
//			free_places += bus_intentions[i].second;
//			bus_intentions.erase(bus_intentions.begin() + i);
//			i--;
//		}
//	}
//
//	if(free_places > 0)
//	{
//		Recalc(curent_time); //Пересчет количества пассажиров на остановке
//
//		//Устанавливаем итератор на текущую остановку
//		vector <int>::iterator cur_stop_it;
//		cur_stop_it = find(bus_stops.begin(), bus_stops.end(), bus_stop_label);
//
//		//Считаем количество пассажиров на остановке желающих проехать по данному маршруту от текущей остановки и далее
//		int passangers_to_this_bus = 0;
//		for(int i = 0; i < remain_passangers.size(); i++)
//		{
//			if((find(cur_stop_it, bus_stops.end(), remain_passangers[i].direction) != bus_stops.end()) && (remain_passangers[i].passangers_number > 0))
//			{
//				passangers_to_this_bus += remain_passangers[i].passangers_number;
//			}
//		}
//
//		if(passangers_to_this_bus <= free_places)
//		{
//			bool flag;
//			for(int i = 0; i < remain_passangers.size(); i++)
//			{
//				flag = false;
//				if(find(cur_stop_it, bus_stops.end(), remain_passangers[i].direction) != bus_stops.end())
//				{
//					pair<int, int> tmp_pair;
//					tmp_pair.first = remain_passangers[i].direction;
//					tmp_pair.second = remain_passangers[i].passangers_number;
//					for(int j = 0; j < bus_intentions.size(); j++)
//					{
//						if(bus_intentions[j].first == tmp_pair.first)
//						{
//							bus_intentions[j].second += tmp_pair.second;
//							remain_passangers[i].passangers_number -= tmp_pair.second;
//							free_places -= tmp_pair.second;
//							passangers_to_this_bus  -= tmp_pair.second;
//							flag = true;
//							break;
//						}
//					}
//					if(!flag)
//					{
//						bus_intentions.push_back(tmp_pair);
//						free_places -= tmp_pair.second;
//						passangers_to_this_bus  -= tmp_pair.second;
//						remain_passangers[i].passangers_number -= tmp_pair.second;
//					}
//				}
//			}
//		}
//		else
//		{
//		
//		}
//
//	}
//	return 0;
//}

int Stop::landLoadPassangers(/*vector <pair<unsigned long, int>> &bus_intentions*/ vector<Passanger> &bus_pussangers, int &free_places, int curent_time, vector <unsigned long> bus_stops, Parameters_ parameters, Bus &bus)
{
	//cout<<"landLoadPassangers started"<<endl;
	//Пассажиры выходят на нужной остановке
	//for(int i = 0; i < bus_intentions.size(); i++)
	//{
	//	if(bus_intentions[i].first == bus_stop_label)
	//	{
	//		free_places += bus_intentions[i].second;
	//		bus_intentions.erase(bus_intentions.begin() + i);
	//		i--;
	//	}
	//}

	//Пассажиры выходят на нужной остановке
	for(int i = 0; i <  bus_pussangers.size(); i++)
	{
		if(bus_pussangers[i].direction == bus_stop_label)
		{
			bus_pussangers[i].arrived_time = curent_time;
			free_places++;
			bus_pussangers.erase(bus_pussangers.begin() + i);
			i--;
		}
	}
	//cout<<"Passangers were landed"<<endl;

	if(free_places > 0)//Если есть свободные места в автобусе
	{
		bool flag= false;

		Recalc(curent_time); //Пересчет количества пассажиров на остановке

		//Поиск отказавшихся от поездки пассажиров и их удаление с остановки
		for(int p = 0; p < passangers.size(); p++)
		{
			if(curent_time - passangers[p].come_time > parameters.wt_max)
			{
				bus.passangers_wt_fines += parameters.rm_restful*(parameters.wt_restful - parameters.wt_no_fine) + parameters.rm_upto_max*(parameters.wt_max - parameters.wt_restful) + parameters.f_refuse;
			}
			passangers.erase(passangers.begin() + p);
			p--;
		}

		//Устанавливаем итератор на текущую остановку
		vector <unsigned long>::iterator cur_stop_it;
		cur_stop_it = find(bus_stops.begin(), bus_stops.end(), bus_stop_label);


		//Считаем количество пассажиров на остановке желающих проехать по данному маршруту от текущей остановки и далее
		int passangers_to_this_bus = 0;
		for(int i = 0; i < passangers.size(); i++)
		{
			if(find(cur_stop_it, bus_stops.end(), passangers[i].direction) != bus_stops.end())
			{
				passangers_to_this_bus++;
			}
		}

		int rnd_passanger = 0;
		while(free_places > 0 && passangers_to_this_bus > 0)
		{
			rnd_passanger = rand()%passangers.size();
			while(find(cur_stop_it, bus_stops.end(), passangers[rnd_passanger].direction) == bus_stops.end())
			{
				rnd_passanger++;
				if(rnd_passanger == passangers.size())
				{
					rnd_passanger = 0;
				}
			}

			Passanger pas;
			pas.come_time = passangers[rnd_passanger].come_time;
			pas.bus_stop_label = passangers[rnd_passanger].bus_stop_label;
			pas.direction = passangers[rnd_passanger].direction;
			pas.bus_enter_time = curent_time;

			if(curent_time - pas.come_time > parameters.wt_no_fine)
			{
				if(curent_time - pas.come_time > parameters.wt_restful)
				{
					double fines = parameters.rm_restful*(parameters.wt_restful - parameters.wt_no_fine) + parameters.rm_upto_max*((curent_time - pas.come_time) - parameters.wt_restful);
					bus.passangers_wt_fines += fines;
				}
				else
				{
					double fines = parameters.rm_restful*(curent_time - pas.come_time);
					bus.passangers_wt_fines += fines;
				}
			}

			bus_pussangers.push_back(pas);
			passangers.erase(passangers.begin() + rnd_passanger);
			free_places--;
			passangers_to_this_bus--;
		}
		
		time_of_last_rout = curent_time;
	}

	//	////Считаем количество пассажиров на остановке желающих проехать по данному маршруту от текущей остановки и далее
	//	//int passangers_to_this_bus = 0;
	//	//for(int i = 0; i < remain_passangers.size(); i++)
	//	//{
	//	//	if(find(cur_stop_it, bus_stops.end(), remain_passangers[i].direction) != bus_stops.end())
	//	//	{
	//	//		passangers_to_this_bus += remain_passangers[i].passangers_number;
	//	//	}
	//	//}
	//	//cout<<"Passangers to this bus: "<<passangers_to_this_bus<<endl;

	//	if(passangers_to_this_bus <= free_places) //Если все могуть сесть
	//	{
	//	//	cout<<"All passangers may set"<<endl;
	//		for(int i = 0; i < remain_passangers.size(); i++)
	//		{
	//			flag = false;
	//			if(find(cur_stop_it, bus_stops.end(), remain_passangers[i].direction) != bus_stops.end())
	//			{
	//				pair<int, int> tmp_pair;
	//				tmp_pair.first = remain_passangers[i].direction;
	//				tmp_pair.second = remain_passangers[i].passangers_number;
	//				for(int j = 0; j < bus_intentions.size(); j++)
	//				{
	//					if(bus_intentions[j].first == tmp_pair.first)
	//					{
	//						bus_intentions[j].second += tmp_pair.second;
	//						remain_passangers[i].passangers_number -= tmp_pair.second;
	//						free_places -= tmp_pair.second;
	//						passangers_to_this_bus  -= tmp_pair.second;
	//						flag = true;
	//						break;
	//					}
	//				}
	//				if(!flag)
	//				{
	//					bus_intentions.push_back(tmp_pair);
	//					free_places -= tmp_pair.second;
	//					passangers_to_this_bus  -= tmp_pair.second;
	//					remain_passangers[i].passangers_number -= tmp_pair.second;
	//				}
	//			}
	//		}
	//		//cout<<"All passangers were setled"<<endl;
	//	}
	//	else //Если пассажиров на остановке больше чем мест в автобусе
	//	{
	//		//cout<<"Passangers number more than bus cupasity"<<endl;
	//		double percent = (double)free_places/(double)passangers_to_this_bus; 
	//		//cout<<"Setling by percent count started"<<endl;
	//		for(int i = 0; i < remain_passangers.size(); i++)
	//		{
	//			/*if(bus_stop_label == 9 && curent_time == 442 && free_places == 1)
	//			cout<<"I: "<<i<<endl;*/
	//			flag = false;
	//			if(find(cur_stop_it, bus_stops.end(), remain_passangers[i].direction) != bus_stops.end())
	//			{
	//				flag = false;//////////////////////////////////////////////////////////////////////////////////////////////
	//				pair<int, int> tmp_pair;
	//				tmp_pair.first = remain_passangers[i].direction;
	//				tmp_pair.second = remain_passangers[i].passangers_number * percent;
	//				if(tmp_pair.second > 0)
	//				{
	//					for(int j = 0; j < bus_intentions.size(); j++)//Поиск пассажиров едущих на ту же остановку
	//					{
	//						if(bus_intentions[j].first == tmp_pair.first)
	//						{
	//							bus_intentions[j].second += tmp_pair.second;
	//							remain_passangers[i].passangers_number -= tmp_pair.second;
	//							free_places -= tmp_pair.second;
	//							passangers_to_this_bus  -= tmp_pair.second;
	//							flag = true;
	//							break;
	//						}
	//					}
	//					if(!flag)//Если в автобусе нет пассажиров едущих на эту остановку
	//					{
	//						bus_intentions.push_back(tmp_pair);
	//						free_places -= tmp_pair.second;
	//						passangers_to_this_bus  -= tmp_pair.second;
	//						remain_passangers[i].passangers_number -= tmp_pair.second;
	//					}
	//				}
	//			}
	//			//cout<<"Setling by percent count finnished"<<endl;
	//		}
	//		if((free_places > 0) && (passangers_to_this_bus > 0))//Если остались свободные места изза округления процентов
	//		{
	//			//cout<<"Random setling started"<<endl;
	//			vector <int> indexes;
	//			//Запоминаем индексы направления которых есть в списке остановок маршрута и на них ожидают пассажиры
	//			for(int i = 0; i < remain_passangers.size(); i++)
	//			{
	//				if((find(cur_stop_it, bus_stops.end(), remain_passangers[i].direction) != bus_stops.end()) && (remain_passangers[i].passangers_number > 0))
	//				{
	//					indexes.push_back(i);
	//				}
	//			}

	//			int tmp_r = 0;
	//			int ind = 0;
	//			//if((bus_stop_label == 9) && (curent_time == 442) && (free_places == 1))
	//			//{
	//			//	char ch;
	//			//	cin>>ch;
	//			//}

	//			while((free_places > 0) && (passangers_to_this_bus > 0))
	//			{					
	//				if(indexes.size() == 0)
	//				{
	//					char ch;
	//					cin>>ch;
	//				}
	//				tmp_r = rand()% indexes.size();
	//				ind = indexes[tmp_r];

	//				pair<int, int> tmp_pair;
	//				tmp_pair.first = remain_passangers[ind].direction;
	//				tmp_pair.second = 1;
	//				remain_passangers[ind].passangers_number--;
	//				if(remain_passangers[ind].passangers_number == 0)
	//				{
	//					indexes[tmp_r] = indexes[indexes.size() - 1];
	//					indexes.erase(indexes.begin() + (indexes.size() - 1));
	//				}
	//				flag = false;
	//				for(int j = 0; j < bus_intentions.size(); j++)//Поиск пассажиров едущих на ту же остановку
	//				{
	//					if(bus_intentions[j].first == tmp_pair.first)
	//					{
	//						bus_intentions[j].second += tmp_pair.second;
	//						free_places -= tmp_pair.second;
	//						passangers_to_this_bus  -= tmp_pair.second;
	//						flag = true;
	//						break;
	//					}
	//				}
	//				if(!flag)//Если в автобусе нет пассажиров едущих на эту остановку
	//				{
	//					bus_intentions.push_back(tmp_pair);
	//					free_places -= tmp_pair.second;
	//					passangers_to_this_bus  -= tmp_pair.second;
	//				}
	//			}
	//			//cout<<"Random setling finished"<<endl;
	//		}
	//		//cout<<"All passangers were setled"<<endl;
	//	}
	//}
	//time_of_last_rout = curent_time;

	//////очистка направлений на остановке по которым нет пассажиров 
	////for(int i = 0; i < remain_passangers.size(); i++)
	////{
	////	if(remain_passangers[i].passangers_number == 0)
	////	{
	////		remain_passangers.erase(remain_passangers.begin() + i);
	////		i--;
	////	}
	////}
	////cout<<"landLoadPassangers finished"<<endl;
	return 0;
}

int Stop::Print()
{
	cout<<"Bus label: "<<bus_stop_label<<endl;
	cout<<"\tDirections by time:"<<endl;
	for(int i = 0; i < vdensity.size(); i++)
	{
		cout<<"\tDirection: "<<vdensity[i].dir<<endl;
		for(int j = 0; j < vdensity[i].vtimedens.size(); j++)
		{
			cout<<"\t  time: "<<vdensity[i].vtimedens[j].time<<" density: "<<vdensity[i].vtimedens[j].density_val<<endl;
		}
	}
	cout<<endl;
	return 0;
}

//double Integrate(vector <pair <int, double>> value_pairs)
//{
//	double I = 0;
//	if(value_pairs.size() > 1)
//	{
//		for(int i = 1; i <  value_pairs.size(); i++)
//		{
//			I += (value_pairs[i].second + value_pairs[i - 1].second)/2 * abs(double(value_pairs[i].first - value_pairs[i - 1].first)); 
//		}
//	}
//
//	return I;
//}


int Stop::Recalc(int current_time) //Перерасчет количества пассажиров на текущей остановке
{
	//cout<<"Recalc started"<<endl;
	//переращет не нужен если переращет уже был произведен на более позднее время либо равное текущему
	if(current_time <= time_of_last_rout)
	{
		return 0;
	}

	int pas_num = 0;

	for(int i = 0; i < vdensity.size(); i++)
	{
		for(int j = 0; j < vdensity[i].vtimedens.size(); j++)
		{
			if(vdensity[i].vtimedens[j].time >= time_of_last_rout && vdensity[i].vtimedens[j].time < current_time) //если время находится в промежутке между прошлым посещением автобуса и текущим временем
			{
				if(vdensity[i].vtimedens[j].density_val > 0)
				{
					pas_num = vdensity[i].vtimedens[j].density_val;

					for(int r = 0; r < pas_num; r++)
					{
						Passanger pas;
						pas.bus_stop_label = bus_stop_label;
						pas.direction = vdensity[i].dir;;
						pas.come_time = current_time;
						passangers.push_back(pas);
					
					}

					//int pointer = -1;
					//for(int d = 0; d < remain_passangers.size(); d++)//Поиск пассажиров которые уже стоят на остановке
					//{
					//	if(remain_passangers[d].direction == vdensity[i].dir)
					//	{
					//		pointer = d;
					//	}
					//}
					//if(pointer == -1)//Если пассажиров по такому направлению еще не было
					//{
					//	intentions inten;//Создаем направление с остановки
					//	inten.direction = vdensity[i].dir;
					//	inten.passangers_number = 0;

					//	remain_passangers.push_back(inten);
					//	pointer = remain_passangers.size() - 1;//Переместили указатель на последний элемент списка
					//}
					//remain_passangers[pointer].passangers_number += pas_num;
				}
			}
		}
	}

	//Обновляем данные об общем количестве пассажиров на остановке
	total_passangers = 0;
	total_passangers = passangers.size();
	//for(int i = 0; i < remain_passangers.size(); i++)
	//{
	//	total_passangers += remain_passangers[i].passangers_number;
	//}

	//cout<<"Recalc finished"<<endl;
	return 0;
}

int Stop::RemainPassangers()	//Возвращает количество пассажиров пришедших до конца дня после последнего автобуса
{
	//Поиск последнего времени прибытия пассажира на остановку
	int last_arrival_time = 0;
	for(int i = 0; i < vdensity.size(); i++)
	{
		for(int j = 0; j < vdensity[i].vtimedens.size(); j++)
		{
			if( vdensity[i].vtimedens[j].time >= last_arrival_time)
			{
				last_arrival_time = vdensity[i].vtimedens[j].time;
			}
		}
	}

	//Пересчет количества пассажиров на остановке на последний момент времени
	Recalc(last_arrival_time);

	return total_passangers;
}


Stop::~Stop(void)
{
}
