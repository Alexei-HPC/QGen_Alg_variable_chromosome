#include "Routes.h"


Routes::Routes(void)
{
}

Routes::Routes(string path)
{
	this->RoutesLoad(path);
}

int Routes::ChromosomeSize()	//Получение длины хромосомы
{
	int ch_size = 0;
	for(int i = 0; i < routes.size(); i++)
	{
		ch_size += routes[i].trip_numbers;
	}
	return ch_size;
}

vector<unsigned long> Split(string str, char ch)	//Получение чисел из строки. числа разделены символом ch
{
	vector<unsigned long> array;
	string digit = "";
	for(int i = 0; i < str.length(); i++)
	{
		if(str[i] == ch)
		{
			array.push_back(atoi(digit.c_str()));
			digit = "";
		}
		else
		{
			digit+= str[i];
		}
	}
	array.push_back(atoi(digit.c_str()));
	return array;
} 

int Routes::RoutesLoad(string path)///Загрузка маршрутов
{
	cout<<"Routes reading started"<<endl;
	TiXmlDocument doc;
	if(!doc.LoadFile(path))//Попытка открыть файл
	{
		cerr << doc.ErrorDesc() << endl;
		exit(1);
		return 1;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if(root == NULL)
	{
		cerr << "Failed to load file: No root element."	<< endl;
		doc.Clear();
		return 1;
	}

	string str_gap;
	for(TiXmlElement* rout_it = root->FirstChildElement(); rout_it != NULL; rout_it = rout_it->NextSiblingElement())
	{
		Rout rout;
		rout.rout_label = atoi(rout_it->Attribute("label"));
		rout.max_interval = atoi(rout_it->Attribute("max_interval"));
		str_gap = rout_it->FirstChildElement("gap")->GetText();
		vector<unsigned long> digits = Split(str_gap,',');
		rout.gap.first = digits[0];
		rout.gap.second = digits[1];
		if(rout.gap.first >= rout.gap.second)	//проверка на корректность границ
		{
			cout<<"Error! time gap is incorrect min: "<<rout.gap.first<<" max: "<<rout.gap.second<<" Borders were swapped."<<endl;
			int tmp = rout.gap.second;
			rout.gap.second = rout.gap.first;
			rout.gap.first = tmp;
		}
		string stops;
		stops = rout_it->FirstChildElement("stops")->GetText();
		rout.stops = Split(stops,',');

		for(TiXmlElement* bus_it = rout_it->FirstChildElement("bus"); bus_it != NULL; bus_it = bus_it->NextSiblingElement())
		{
			bus bs;
			bs.capacity = atoi(bus_it->Attribute("capacity"));
			bs.lambda = atoi(bus_it->Attribute("lambda"));
			bs.bus_label = atoi(bus_it->Attribute("label"));
			rout.buses.push_back(bs);
		}
		rout.trip_numbers = rout.buses.size();
		routes.push_back(rout);
	}

	cout<<"Routes reading completed"<<endl;
	return 0;
}

int Routes::GetGaps(vector< pair <int, pair<int, int>>> &routs_gaps) //заполнение вектора промежутками работы маршрутов
{

	for(int i = 0; i < routes.size(); i++)
	{
		routes[i].trip_numbers;
		routs_gaps.push_back( pair <int, pair<int, int>>(routes[i].trip_numbers, routes[i].gap));
	}
	return 0;
}

int Routes::PrintRoutes()
{
	for(int i = 0; i < routes.size(); i++)
	{
		cout<<"Routes number: "<<routes.size()<<endl;
		cout<<"Rout label: "<<routes[i].rout_label<<endl;
		//cout<<"Gap: "<<routes[i].gap.first<<" "<<routes[i].gap.second<<endl;
		for(int j = 0; j < routes[i].buses.size(); j++)
		{
			cout<<"Bus: "<<j<<" Capasity: "<<routes[i].buses[j].capacity<<" lambda: "<<routes[i].buses[j].lambda<<endl;
		}

		cout<<"Stops: ";
		for(int j = 0; j < routes[i].stops.size(); j++)
		{
			cout<<routes[i].stops[j]<<" ";
		}
		cout<<endl;
	}
	return 0;
}

Routes::~Routes(void)
{
}
