#include "MinimalDriveTime.h"


MinimalDriveTime::MinimalDriveTime(void)
{
}

MinimalDriveTime::MinimalDriveTime(string path, double min_dr_time)
{
	minimal_drive_time = min_dr_time;
	MinDriveTimeInit(path);
}

//Для сортировки времени
bool compare_time(pair<int, double> i,pair<int, double> j) 
{
	return (i.first < j.first);
}

//int MinimalDriveTime::MinDriveTimeInit(string path)
//{
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
//	time_graph.clear();
//	int a, b;
//	double cost;
//	int time_val = 0;
//
//	for(TiXmlElement* edge_it = root->FirstChildElement(); edge_it != NULL; edge_it = edge_it->NextSiblingElement())
//	{
//		a = atoi(edge_it->Attribute("a"));
//		b = atoi(edge_it->Attribute("b"));
//		edge ed;
//		for(TiXmlElement* time = edge_it->FirstChildElement(); time != NULL; time = time->NextSiblingElement())
//		{
//			time_val = atoi(time->Attribute("time"));
//			cost = atoi(time->Attribute("min_time"));
//			ed.a = a;
//			ed.b = b;
//			ed.time_drivetime.push_back(pair<int, double>(time_val, cost));
//		}
//
//		time_graph.push_back(ed);
//	}
//
//	for(int i = 0; i < time_graph.size(); i++)
//	{
//		sort(time_graph[i].time_drivetime.begin(), time_graph[i].time_drivetime.end(), compare_time);
//	}
//
//	return 0;
//}
//
int MinimalDriveTime::MinDriveTimeInit(string path)
{
	cout<<"Driving times reading started"<<endl;
	string line;
	int counter = 0; 
	string tmpstr = "";
	unsigned long a, b;
	bool flag = false;
	ifstream filewithtimes(path);

	if(filewithtimes.is_open())//проверяем, открыт ли файл
	{
		while (getline(filewithtimes,line))//Считываем построчно
		{
			edge ed;
			istringstream iss(line);
			iss >> tmpstr;
			ed.a = atoi(tmpstr.c_str());
			iss >> tmpstr;
			ed.b = atoi(tmpstr.c_str());


			pair<int, double> time_drive;
			iss >> tmpstr;
			time_drive.first = atoi(tmpstr.c_str());
			iss >> tmpstr;
			time_drive.second = (double)(atoi(tmpstr.c_str())/60) + (double)(atoi(tmpstr.c_str())%60); //Получаем минуты
			flag = false;

			for(int i = 0; i < time_graph.size(); i++)//Ищем такое же ребро внесенное ранее
			{
				if(time_graph[i].a == ed.a && time_graph[i].b == ed.b)
				{
					time_graph[i].time_drivetime.push_back(time_drive);
					flag = true;
				}
			}

			if(!flag)//Если ранее такого ребра не было
			{
				ed.time_drivetime.push_back(time_drive);
				time_graph.push_back(ed);
			}
		}
		
		for(int i = 0; i < time_graph.size(); i++)
		{
			sort(time_graph[i].time_drivetime.begin(), time_graph[i].time_drivetime.end(), compare_time);
		}
		
		filewithtimes.close();
	}
	else
	{
		cout<<"Error! No such file: "<<path<<endl;
		exit(1);
		return 1;
	}

	cout<<"Driving times reading completed"<<endl;
	return 0;
}

int MinimalDriveTime::GetMinTime(unsigned long a, unsigned long b, int time)
{
	for(int i = 0; i < time_graph.size(); i++)
	{
		if(time_graph[i].a == a && time_graph[i].b == b)
		{
			for(int j = 0; j < time_graph[i].time_drivetime.size(); j++)
			{
				if(time_graph[i].time_drivetime[j].first <= time)
				{
					return time_graph[i].time_drivetime[j].second;
				}
			}
		}
	}
	cout<<"Error! No such edge or time gap a: "<<a<<" b: "<<b<<" time: "<<time<<endl;
	return minimal_drive_time;
}

int MinimalDriveTime::MinTimesPrint()
{
	for(int i = 0; i < time_graph.size(); i++)
	{
		cout<<"Edge: "<<time_graph[i].a<<" "<<time_graph[i].b<<endl;
		for(int j = 0; j < time_graph[i].time_drivetime.size(); j++)
		{
			cout<<"\ttime: "<<time_graph[i].time_drivetime[j].first<<" minimal drive time: "<<time_graph[i].time_drivetime[j].second<<endl;
		}
	}
	return 0;
}


MinimalDriveTime::~MinimalDriveTime(void)
{
}
