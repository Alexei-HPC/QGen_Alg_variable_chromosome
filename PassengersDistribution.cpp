#include "PassengersDistribution.h"


PassengersDistribution::PassengersDistribution(void)
{

}

PassengersDistribution::PassengersDistribution(string path)
{
	this->Distribution_load(path);
}

int PassengersDistribution::PassNumber(unsigned long label_of_bus_stop, int time)
{
	for(unsigned long i = 0; i < array_of_stops.size(); i++)
	{
		if(label_of_bus_stop == array_of_stops[i].label_of_bus_stop)
		{
			for(unsigned long j = 0; j < array_of_stops[i].intentions_by_time.size(); j++)
			{
				if(array_of_stops[i].intentions_by_time[j].first == time)
				{
					return array_of_stops[i].intentions_by_time[j].second.total_passangers;
				}
			}
		}
	}
	return 0;
}


int PassengersDistribution::Distribution_load(string path)
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

	for(TiXmlElement* bus_stop_it = root->FirstChildElement(); bus_stop_it != NULL; bus_stop_it = bus_stop_it->NextSiblingElement())
	{
		bus_stop bs;
		bs.label_of_bus_stop = atoi(bus_stop_it->Attribute("label"));
		for(TiXmlElement* time_it = bus_stop_it->FirstChildElement(); time_it != NULL; time_it = time_it->NextSiblingElement())
		{
			people peop;
			peop.total_passangers = 0;
			peop.comed_passangers = 0;
			pair<int, people> pairv;
			for(TiXmlElement* intention_it = time_it->FirstChildElement(); intention_it != NULL; intention_it = intention_it->NextSiblingElement())
			{
				intention inten;
				inten.passangers_direction = atoi(intention_it->Attribute("direction"));
				inten.passangers_number = atoi(intention_it->GetText());
				peop.intentions.push_back(inten);
				peop.total_passangers += inten.passangers_number;
			}
			peop.comed_passangers = peop.total_passangers;
			peop.remain_passangers = 0;
			pairv.first = atoi(time_it->Attribute("value"));
			pairv.second = peop;
			bs.intentions_by_time.push_back(pairv);
		}
		array_of_stops.push_back(bs);
	}

/*
	int time = -1;
	int label = -1;
	int total_pass = 0;

	for(TiXmlElement* moment = root->FirstChildElement(); moment != NULL; moment = moment->NextSiblingElement())
	{
		time = atoi(moment->Attribute("time"));
		cout<<"Moment"<<endl;
		cout<<time<<endl;
		for(TiXmlElement* bus_stop = moment->FirstChildElement(); bus_stop != NULL; bus_stop = bus_stop->NextSiblingElement())
		{
			label = atoi(bus_stop->Attribute("label"));
			cout<<"\tlabel: "<<label<<endl;
			total_pass = atoi(bus_stop->FirstChildElement("total_passengers")->GetText());
			cout<<"\ttotal_pass: "<<total_pass<<endl;
			for(TiXmlElement* direction = bus_stop->FirstChildElement("directions")->FirstChildElement(); direction != NULL; direction = direction->NextSiblingElement())
			{
				cout<<direction->FirstChildElement("intention")->GetText()<<endl;
				cout<<direction->FirstChildElement("passengers_number")->GetText()<<endl;
			}
		}
	}*/
	return 0;
}

int PassengersDistribution::PrintDisrtibution()
{
	for(unsigned long i = 0; i < array_of_stops.size(); i++)
	{
		PrintBusStop(array_of_stops[i].label_of_bus_stop);
	}
	return 0;
}


int PassengersDistribution::PrintBusStop(unsigned long bus_stop)
{
	for(unsigned long i = 0; i < array_of_stops.size(); i++)
	{
		if(bus_stop == array_of_stops[i].label_of_bus_stop)
		{
			cout<<"Bus stop: "<<array_of_stops[i].label_of_bus_stop<<endl;
			for(int j = 0; j < array_of_stops[i].intentions_by_time.size(); j++)
			{
				cout<<" time: "<<array_of_stops[i].intentions_by_time[j].first<<endl;
				cout<<"   total passangers: "<<array_of_stops[i].intentions_by_time[j].second.total_passangers<<endl;
				cout<<"   comed passangers: "<<array_of_stops[i].intentions_by_time[j].second.comed_passangers<<endl;
				cout<<"   remain passangers: "<<array_of_stops[i].intentions_by_time[j].second.remain_passangers<<endl;
				cout<<"    intensions: "<<endl;
				for(int k = 0; k < array_of_stops[i].intentions_by_time[j].second.intentions.size(); k++)
				{
					cout<<"    direction: "<<array_of_stops[i].intentions_by_time[j].second.intentions[k].passangers_direction;
					cout<<"    passangers number: "<<array_of_stops[i].intentions_by_time[j].second.intentions[k].passangers_number<<endl;
				}
			}
		}
	}
	return 0;
}

PassengersDistribution::~PassengersDistribution(void)
{
}



//TiXmlDocument doc;
//if(!doc.LoadFile(path))//Попытка открыть файл
//{
//	cerr << doc.ErrorDesc() << endl;
//	return 1;
//}

//TiXmlElement* root = doc.FirstChildElement();
//if(root == NULL)
//{
//	cerr << "Failed to load file: No root element."	<< endl;
//	doc.Clear();
//	return 1;
//}

//const char* attr;
//string str = "";
//int time = -1;
//int label = -1;
//int total_pass = 0;

//for(TiXmlElement* moment = root->FirstChildElement(); moment != NULL; moment = moment->NextSiblingElement())
//{
//	time = atoi(moment->Attribute("time"));
//	cout<<"Moment"<<endl;
//	cout<<time<<endl;
//	for(TiXmlElement* bus_stop = moment->FirstChildElement(); bus_stop != NULL; bus_stop = bus_stop->NextSiblingElement())
//	{
//		label = atoi(bus_stop->Attribute("label"));
//		cout<<"\tlabel: "<<label<<endl;
//		total_pass = atoi(bus_stop->FirstChildElement("total_passengers")->GetText());
//		cout<<"\ttotal_pass: "<<total_pass<<endl;
//		for(TiXmlElement* direction = bus_stop->FirstChildElement("directions")->FirstChildElement(); direction != NULL; direction = direction->NextSiblingElement())
//		{
//			cout<<direction->FirstChildElement("intention")->GetText()<<endl;
//			cout<<direction->FirstChildElement("passengers_number")->GetText()<<endl;
//		}
//	}
//}