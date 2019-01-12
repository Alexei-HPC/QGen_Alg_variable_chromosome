#include "Individual.h"


Individual::Individual(void)
{
	chromosome_size = 0;
	fitness_value = 0;
}

int Getrandvalue(int min, int max)
{
	int rnd_val = 0;
	rnd_val = min + (int)floor((double(rand())/RAND_MAX)*(max - min + 1));
	return rnd_val;
}

Individual::Individual(int ch_size, vector< pair <int, pair<int, int>>> gaps)
{
	chromosome_size = ch_size;
	fitness_value = 0;
	//this->lengths.resize(plengths.size());
	//for (int i = 0; i < plengths.size(); i++)
	//{	
	//	this->lengths[i] = plengths[i];
	//}

	for(int i = 0; i < gaps.size(); i++)
	{
		pair<int, int> a(gaps[i].second.first,gaps[i].second.second);
		routs_gaps.push_back(pair<int, pair <int, int>>(gaps[i].first, a));
	}

	chromosome = new int[chromosome_size];
	int counter = 0;



	for(int i = 0; i < routs_gaps.size(); i++)	//По количеству маршрутов
	{
		for(int j = 0; j < routs_gaps[i].first; j++)	//по количеству рейсов
		{
			chromosome[counter] = Getrandvalue(routs_gaps[i].second.first, routs_gaps[i].second.second);// routs_gaps[i].second.first + (int)floor((double(rand())/RAND_MAX)*(routs_gaps[i].second.second - routs_gaps[i].second.first + 1));
			counter++;
		}
	}

	//for(int i = 0; i < chromosome_size; i++)
	//{
	//	chromosome[i] = min + (int)floor((double(rand())/RAND_MAX)*(max - min + 1));//rand() % 1441;//В сутках 1440 минут
	//}
	//min + (int)floor((double(rand())/RAND_MAX)*(max - min + 1));
}

Individual::Individual(const Individual &tmp)
{
	this->chromosome_size = tmp.chromosome_size;
	this->fitness_value = tmp.fitness_value;
	
	/*this->lengths.resize(tmp.lengths.size());
	for(int i = 0; i < tmp.lengths.size(); i++)
	{
		this->lengths[i] = tmp.lengths[i];
	}*/

	this->routs_gaps.clear();

	for(int i = 0; i < tmp.routs_gaps.size(); i++)
	{
		pair<int, int> a(tmp.routs_gaps[i].second.first,tmp.routs_gaps[i].second.second);
		routs_gaps.push_back(pair<int, pair <int, int>>(tmp.routs_gaps[i].first, a));
	}

	this->chromosome = new int[tmp.chromosome_size];

	for(int i = 0; i < chromosome_size; i++)
	{
		this->chromosome[i] = tmp.chromosome[i];
	}
}

int Individual::PrintIndivid(void)
{
	for(int i = 0; i < chromosome_size; i++)
	{
		cout<<chromosome[i]<<" ";
	}
	cout<<endl;
	return 0;
}

int Individual::Mutation(double probability, int mut_number)
{
	double Pm = probability; //Вероятность мутации

	if(chromosome_size <= 0)//Выход если нечего мутировать
	{
		cout<<"Error! No genes or chromosomes."<<endl;
		return 1;
	} 

	vector <int> mutation_points;
	for(int i = 0; i < mut_number; i++)
	{
		mutation_points.push_back(rand()%chromosome_size);
	}
	sort(mutation_points.begin(), mutation_points.end());

	int pointer = 0;
	int chromosome_pointer = 0;
	for(int i = 0; i < routs_gaps.size(); i++)	//По количеству маршрутов
	{
		for(int j = 0; j < routs_gaps[i].first; j++)	//по количеству рейсов
		{
			if(chromosome_pointer == mutation_points[pointer])
			{
				chromosome[chromosome_pointer] = (double)rand()/RAND_MAX < Pm ? Getrandvalue(routs_gaps[i].second.first, routs_gaps[i].second.second) : chromosome[chromosome_pointer];
				pointer = ++pointer % mut_number;
			}
			chromosome_pointer++;
		}
	}

	return 0;
}

Individual::~Individual(void)
{
	delete [] chromosome;
}
