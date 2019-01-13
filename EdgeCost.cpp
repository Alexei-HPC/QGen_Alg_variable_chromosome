#include "EdgeCost.h"


EdgeCost::EdgeCost(void)
{
}

EdgeCost::EdgeCost(string path, double min_cost)
{
	minimal_cost = min_cost;
	EdgeCostInit(path);
}

//для сортировки промежутков стоимостей проезда
bool compare_times_(pair<int, double> i,pair<int, double> j) 
{
	return (i.first < j.first);
}

//Считывание данных из файла о стоимостях
int EdgeCost::EdgeCostInit(string path)
{
	cout<<"Costs reading started"<<endl;
	TiXmlDocument doc;
	if(!doc.LoadFile(path))//Попытка открыть файл
	{
		cerr << doc.ErrorDesc() << endl;//Выводим ошибку открытия файла
		exit(1);
		return 1;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if(root == NULL)
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
		return 1;
	}

	cost_graph.clear();
	unsigned long a, b;
	double cost;
	int time_val = 0;

	for(TiXmlElement* edge_it = root->FirstChildElement(); edge_it != NULL; edge_it = edge_it->NextSiblingElement())
	{
		a = atoi(edge_it->Attribute("a"));//Считали данные о ребре
		b = atoi(edge_it->Attribute("b"));
		edge ed;
		for(TiXmlElement* time = edge_it->FirstChildElement(); time != NULL; time = time->NextSiblingElement())
		{
			time_val = atoi(time->Attribute("time"));
			cost = atoi(time->Attribute("cost"));
			ed.a = a;
			ed.b = b;
			ed.time_cost.push_back(pair<int, double>(time_val, cost));
		}
		cost_graph.push_back(ed);
	}

	for(unsigned long i = 0; i < cost_graph.size(); i++)//Сортируем по временам
	{
		sort(cost_graph[i].time_cost.begin(), cost_graph[i].time_cost.end(), compare_times_);
	}
	
	cout<<"Costs reading completed"<<endl;
	return 0;
}

//Возвращаем минимальную стоимосить проезда по ребру а b  в момент времени t
double EdgeCost::GetEdgeCost(unsigned long a, unsigned long b, int time)
{
	for(unsigned long i = 0; i < cost_graph.size(); i++)
	{
		if(cost_graph[i].a == a && cost_graph[i].b == b)//Ищем нужное ребро
		{
			for(unsigned long j = 0; j < cost_graph[i].time_cost.size(); j++)
			{
				if(cost_graph[i].time_cost[j].first <= time)//Ищем нужный временной интервал
				{
					return cost_graph[i].time_cost[j].second;
				}
			}
		}
	}
	cout << "Error! GetEdgeCost: No such gap in data. a: " << a << " b: " << b << " time: " << time << endl;
	return minimal_cost;
}

//Выводим информацию о стоимостях проезда по ребрам в различные промежутки времени
int EdgeCost::EdgeCostsPrint()
{
	for(unsigned long i = 0; i < cost_graph.size(); i++)
	{
		cout << "Edge: " << cost_graph[i].a << " " << cost_graph[i].b << endl;
		for(unsigned long j = 0; j < cost_graph[i].time_cost.size(); j++)
		{
			cout << "\ttime: " << cost_graph[i].time_cost[j].first << " cost: " << cost_graph[i].time_cost[j].second << endl;
		}
	}
	return 0;
}


EdgeCost::~EdgeCost(void)
{
}
