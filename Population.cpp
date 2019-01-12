#include "Population.h"


Population::Population(void)
{
	population_size = 0;
}

Population::Population(int pop_size, int ch_size, vector< pair <int, pair<int, int>>> routs_gaps)
{
	if(pop_size <= 0)
	{
		cout<<"Error! Empty population"<<endl;
		return;
	}
	
	chromosome_size = ch_size;
	population_size = pop_size;
	individuals.clear();
	for(int i = 0; i < population_size; i++)
	{
		Individual new_individ(chromosome_size, routs_gaps);
		individuals.push_back(new_individ);
	}
}

int Population::Mutation(double probability, int mut_number)
{
	for(int i = 0; i < population_size; i++)//ћутацию первой строки не проводим
	{
		individuals[i].Mutation(probability, mut_number);
	}
	return 0;
}

int Population::PrintPopulation(void)//¬ывод содержимого хромосом всех особей
{
	for(int i = 0; i < population_size; i++)
	{
		cout<<"Individ "<< i + 1 <<endl;
		individuals[i].PrintIndivid();
	}
	return 0;
}

int Population::Selection(int truncation)
{
	if(truncation <= 1)
	{
		cout<<"Error! Few elements to cut-off";
		return 0;
	}

	if(truncation > population_size)
	{
		cout<<"Error! Truncation elements more than population size"<<endl;
		return 0;
	}

	double **temp_arr = new double*[population_size];
	for(int i = 0; i < population_size; i++)
	{
		temp_arr[i] = new double [2];
		temp_arr[i][0] = individuals[i].fitness_value;
		temp_arr[i][1] = i;
	}

	//—ортировка
	double t1 = 0;
	double t2 = 0;
	/* внешний цикл. i Ц позици€ первого неотсортированного элемента на данной итерации */
	for (int i = 0; i < population_size - 1; i++) {
		int min = i; /* min Ц позици€ минимального элемента */

		/* внутренний цикл. если найден элемент строго меньший текущего минимального, записываем его индекс как минимальный */
		for(int j = i + 1; j < population_size; j++) {
			if(temp_arr[j][0] < temp_arr[min][0])
				min = j;
		}
		if(min != i) /* минимальный элемент не €вл€етс€ первым неотсортированным, обмен нужен */
		{
			t1 = temp_arr[i][0];
			t2 = temp_arr[i][1];
			temp_arr[i][0] = temp_arr[min][0];
			temp_arr[i][1] = temp_arr[min][1];
			temp_arr[min][0] = t1;
			temp_arr[min][1] = t2;

		}
	}

	//‘ормирование новой попул€ции
	vector<Individual> new_ind;
	int a = 0;
	int b = 0;
	int point = 0;

	for(int i = 0; i < truncation; i++)//ѕеренос из старой попул€ции в новую самых успешных особей
	{
		Individual individ(chromosome_size, routs_gaps);
		for(int j = 0; j < chromosome_size; j++)
		{
			individ.chromosome[j] = individuals[temp_arr[i][1]].chromosome[j];
		}
		new_ind.push_back(individ);
	}

	for(int i = 0; /*i < population_size ||*/ new_ind.size() < population_size; i++)//ƒобавление в новую попул€цию новых особей полученных скрещиваниемсамых успешных
	{
		a = rand() % truncation;
		b = rand() % truncation;
		while(a == b)
		{
			b = rand() % truncation;
		}
		a = temp_arr[a][1];
		b = temp_arr[b][1];

		point = rand() % chromosome_size;
		Individual individ_a(chromosome_size, routs_gaps);
		Individual individ_b(chromosome_size, routs_gaps);

		for(int k = 0; k < point; k++)
		{
			individ_a.chromosome[k] = individuals[a].chromosome[k];
			individ_b.chromosome[k] = individuals[b].chromosome[k];
		}

		for(int k = point; k < chromosome_size; k++)
		{
			individ_a.chromosome[k] = individuals[b].chromosome[k];
			individ_b.chromosome[k] = individuals[a].chromosome[k];
		}
		if(new_ind.size() < population_size)
		{
			new_ind.push_back(individ_a);
		}
		if(new_ind.size() < population_size)
		{
			new_ind.push_back(individ_b);
		}
	}

	for(int i = 0; i < population_size; i++)//«амена старой попул€ции новой попул€цией
	{
		for(int j = 0; j < chromosome_size; j++)
		{
			individuals[i].chromosome[j] = new_ind[i].chromosome[j];
		}
	}

	for(int i = 0; i < population_size; i++)
	{
		delete [] temp_arr[i];
	}
	delete [] temp_arr;

	return 0;
}

int Population::Crossover(double probability)//—крещивание
{
	double Pc = probability; //¬еро€тность скрещивани€
	vector<Individual> new_ind;
	if(this->population_size <= 1 || chromosome_size == 0 )//¬ыход если нечего мутировать
	{
		cout<<"Error! No material to crossover."<<endl;
		return 1;
	} 

	int a, b;
	int point = 0;
	double temp = 0;
	for(int i = 0; /*i < population_size ||*/ new_ind.size() < population_size; i++)
	{
		a = rand()%population_size;
		b = rand()%population_size;
		while(a == b)
		{
			b = rand()%population_size;
		}

		if((double)rand()/RAND_MAX < Pc)
		{
			Individual individ_a(chromosome_size, routs_gaps);
			Individual individ_b(chromosome_size, routs_gaps);

			point = rand() % chromosome_size;

			for(int k = 0; k < point; k++)
			{
				individ_a.chromosome[k] = individuals[a].chromosome[k];
				individ_b.chromosome[k] = individuals[b].chromosome[k];
			}
			for(int k = point; k < chromosome_size; k++)
			{
				individ_b.chromosome[k] = individuals[a].chromosome[k];
				individ_a.chromosome[k] = individuals[b].chromosome[k];
			}
			new_ind.push_back(individ_a);
			new_ind.push_back(individ_b);
		}
		else
		{
			Individual individ_a(chromosome_size, routs_gaps);
			Individual individ_b(chromosome_size, routs_gaps);

			for(int k = 0; k < chromosome_size; k++)
			{
				individ_a.chromosome[k] = individuals[a].chromosome[k];
				individ_b.chromosome[k] = individuals[b].chromosome[k];
			}
			new_ind.push_back(individ_a);
			new_ind.push_back(individ_b);
		}
	}

	for(int i = 0; i < population_size; i++)//«амена старой попул€ции новой попул€цией
	{
		for(int j = 0; j < chromosome_size; j++)
		{
			individuals[i].chromosome[j] = new_ind[i].chromosome[j];
		}
	}

	return 0;
}


Population::~Population(void)
{
	//for(int i = 0; i < population_size; i++)
	//{
	//	individuals[i].~Individual();
	//}
}
